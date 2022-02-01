#!/usr/bin/env python3
#
# ======- github-automation - LLVM GitHub Automation Routines--*- python -*--==#
#
# Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
# ==-------------------------------------------------------------------------==#

import argparse
from git import Repo
import github
import os
import re
import requests
import sys
from typing import *

def run_graphql_query(query: str, token: str) -> dict:
    headers = {
        'Authorization' : 'bearer {}'.format(token),
    }
    request = requests.post(
        url = 'https://api.github.com/graphql',
        json = {"query" : query },
        headers = headers)

    if request.status_code == 200:
        return request.json()['data']
    else:
        raise Exception(
            "Failed to run graphql query\nquery: {}\nerror: {}".format(query, request.json()))

class IssueSubscriber:

    @property
    def team_name(self) -> str:
        return self._team_name

    def __init__(self, token:str, repo:str, issue_number:int, label_name:str):
        self.repo = github.Github(token).get_repo(repo)
        self.org = github.Github(token).get_organization(self.repo.organization.login)
        self.issue = self.repo.get_issue(issue_number)
        self._team_name = 'issue-subscribers-{}'.format(label_name).lower()

    def run(self) -> bool:
        for team in self.org.get_teams():
            if self.team_name != team.name.lower():
                continue
            comment = '@llvm/{}'.format(team.slug)
            self.issue.create_comment(comment)
            return True
        return False

def setup_llvmbot_git(git_dir = '.'):
    """
    Configure the git repo in `git_dir` with the llvmbot account so
    commits are attributed to llvmbot.
    """
    repo = Repo(git_dir)
    with repo.config_writer() as config:
        config.set_value('user', 'name', 'llvmbot')
        config.set_value('user', 'email', 'llvmbot@llvm.org')

class ReleaseWorkflow:

    """
    This class implements the sub-commands for the release-workflow command.
    The current sub-commands are:
        * create-branch
        * create-pull-request

    The execute_command method will automatically choose the correct sub-command
    based on the text in stdin.
    """

    def __init__(self, token:str, repo:str, issue_number:int,
                       branch_repo_name:str, branch_repo_token:str,
                       llvm_project_dir:str):
        self._token = token
        self._repo_name = repo
        self._issue_number = issue_number
        self._branch_repo_name = branch_repo_name
        if branch_repo_token:
            self._branch_repo_token = branch_repo_token
        else:
            self._branch_repo_token = self.token
        self._llvm_project_dir = llvm_project_dir
    
    @property
    def token(self) -> str:
        return self._token

    @property
    def repo_name(self) -> str:
        return self._repo_name

    @property
    def issue_number(self) -> int:
        return self._issue_number
    
    @property
    def branch_repo_name(self) -> str:
        return self._branch_repo_name

    @property
    def branch_repo_token(self) -> str:
        return self._branch_repo_token

    @property
    def llvm_project_dir(self) -> str:
        return self._llvm_project_dir

    def get_issue(self):
        repo = github.Github(self.token).get_repo(self.repo_name)
        issue = repo.get_issue(self.issue_number)
        return issue

    def get_push_url(self):
        return 'https://{}@github.com/{}'.format(self.branch_repo_token, self.branch_repo_name)

    def get_branch_name(self):
        return 'issue{}'.format(self.issue_number)

    def get_release_branch_for_issue(self):
        issue = self.get_issue()
        m = re.search('branch: (.+)',issue.milestone.description)
        if m:
            return m.group(1)
        return None 

    def print_release_branch(self):
        print(self.get_release_branch_for_issue())

    def issue_notify_branch(self):
        issue = self.get_issue()
        issue.create_comment('/branch {}/{}'.format(self.branch_repo_name, self.get_branch_name()))

    def issue_notify_pull_request(self, pull):
        issue = self.get_issue()
        issue.create_comment('/pull-request {}#{}'.format(self.branch_repo_name, pull.number))

    def get_action_url(self):
        if os.getenv('CI'):
            return 'https://github.com/{}/actions/runs/{}'.format(os.getenv('GITHUB_REPOSITORY'), os.getenv('GITHUB_RUN_ID'))
        return ""

    def issue_notify_cherry_pick_failure(self, commit:str):
        message = "<!--IGNORE-->\nFailed to cherry-pick: {}\n\n".format(commit)
        action_url = self.get_action_url()
        if action_url:
            message += action_url + "\n\n"
        message += "Please manually backport the fix and push it to your github fork.  Once this is done, please add a comment like this:\n\n`/branch <user>/<repo>/<branch>`"
        issue = self.get_issue()
        issue.create_comment(message)
        issue.add_to_labels('release:cherry-pick-failed')

    def issue_notify_pull_request_failure(self, branch):
        message = "Failed to create pull request for {} ".format(branch)
        message += self.get_action_url()
        issue = self.get_issue()
        issue.create_comment(message)


    def create_branch(self, commits:List[str]) -> bool:
        """
        This function attempts to backport `commits` into the branch associated
        with `self.issue_number`.

        If this is successful, then the branch is pushed to `self.branch_repo_name`, if not,
        a comment is added to the issue saying that the cherry-pick failed.
        
        :param list commits: List of commits to cherry-pick.

        """
        print('cherry-picking', commits)
        branch_name = self.get_branch_name()
        local_repo = Repo(self.llvm_project_dir)
        local_repo.git.checkout(self.get_release_branch_for_issue())

        for c in commits:
            try:
                local_repo.git.cherry_pick('-x', c)
            except Exception as e:
                self.issue_notify_cherry_pick_failure(c)
                raise e

        push_url = self.get_push_url()
        print('Pushing to {} {}'.format(push_url, branch_name))
        local_repo.git.push(push_url, 'HEAD:{}'.format(branch_name))

        self.issue_notify_branch()
        return True


    def create_pull_request(self, owner:str, branch:str):
        """
        reate a pull request in `self.branch_repo_name`.  The base branch of the
        pull request will be choosen based on the the milestone attached to
        the issue represented by `self.issue_number`  For example if the milestone
        is Release 13.0.1, then the base branch will be release/13.x. `branch`
        will be used as the compare branch.
        https://docs.github.com/en/get-started/quickstart/github-glossary#base-branch
        https://docs.github.com/en/get-started/quickstart/github-glossary#compare-branch
        """
        repo = github.Github(self.token).get_repo(self.branch_repo_name)
        issue_ref = '{}#{}'.format(self.repo_name, self.issue_number)
        pull = None
        try:
            pull = repo.create_pull(title='PR for {}'.format(issue_ref),
                                    body='resolves {}'.format(issue_ref),
                                    base=self.get_release_branch_for_issue(),
                                    head='{}:{}'.format(owner, branch),
                                    maintainer_can_modify=False)
        except Exception as e:
            self.issue_notify_pull_request_failure(branch)
            raise e
        self.issue_notify_pull_request(pull)
        return True
        

    def execute_command(self) -> bool:
        """
        This function reads lines from STDIN and executes the first command
        that it finds.  The 2 supported commands are:
        /cherry-pick commit0 <commit1> <commit2> <...>
        /branch <owner>/<repo>/<branch>
        """
        for line in sys.stdin:
            line.rstrip()
            m = re.search("/([a-z-]+)\s(.+)", line)
            if not m:
                continue
            command = m.group(1)
            args = m.group(2)

            if command == 'cherry-pick':
                return self.create_branch(args.split())

            if command == 'branch':
                m = re.match('([^/]+)/([^/]+)/(.+)', args)
                if m:
                    owner = m.group(1)
                    branch = m.group(3)
                    return self.create_pull_request(owner, branch)

        print("Do not understand input:")
        print(sys.stdin.readlines())
        return False

def create_cherry_pick_request_from_closed_issue(issue:int, token:str):
    """
    Request a cherry-pick of the commits that fixed `issue` by creating a new
    issue and attaching it to the release milestone.
    """

    query = """
        query {
            repository(owner: "llvm", name: "llvm-project") {
                issue(number:""" f"""{issue}""" """) {
                    timelineItems (itemTypes: [CLOSED_EVENT], last: 1) {
                        edges {
                            node {
                                __typename
                                ... on ClosedEvent {
                                    closer {
                                        ... on Commit {
                                            author {
                                                user {
                                                    login
                                                }
                                            }
                                            committer {
                                                user {
                                                    login
                                                }
                                            }
                                            oid
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }"""

    data = run_graphql_query(query, token)
    event = data['repository']['issue']['timelineItems']['edges']
    authors = []
    committers = []
    commits = []
    for e in event:
        closer = e['node']['closer']
        authors.append(closer['author']['user']['login'])
        committers.append(closer['committer']['user']['login'])
        commits.append(closer['oid'])
    assignees = list(set(authors + committers))
    print(authors, committers, assignees, commits)
    repo = github.Github(token).get_repo('llvm/llvm-project')
    # Find the most recent release milestone
    milestone = None
    for m in repo.get_milestones(state='open'):
        if re.search('branch: release', m.description):
            milestone = m
            break

    gh_issue = repo.create_issue(title='Cherry-pick fixes for issue#{}'.format(issue),
                                 assignees=assignees,
                                 milestone=milestone)
    gh_issue.create_comment('{} What do you think about backporting {}?\n\n/cherry-pick {}'.format(' '.join(['@' + login for login in assignees]), 'this' if len(commits) == 1 else 'these', ' '.join(commits)))



parser = argparse.ArgumentParser()
parser.add_argument('--token', type=str, required=True, help='GitHub authentiation token')
parser.add_argument('--repo', type=str, default=os.getenv('GITHUB_REPOSITORY', 'llvm/llvm-project'),
                    help='The GitHub repository that we are working with in the form of <owner>/<repo> (e.g. llvm/llvm-project)')
subparsers = parser.add_subparsers(dest='command')

issue_subscriber_parser = subparsers.add_parser('issue-subscriber')
issue_subscriber_parser.add_argument('--label-name', type=str, required=True)
issue_subscriber_parser.add_argument('--issue-number', type=int, required=True)

release_workflow_parser = subparsers.add_parser('release-workflow')
release_workflow_parser.add_argument('--llvm-project-dir', type=str, default='.', help='directory containing the llvm-project checout')
release_workflow_parser.add_argument('--issue-number', type=int, required=True, help='The issue number to update')
release_workflow_parser.add_argument('--branch-repo-token', type=str,
                                     help='GitHub authentication token to use for the repository where new branches will be pushed. Defaults to TOKEN.')
release_workflow_parser.add_argument('--branch-repo', type=str, default='llvmbot/llvm-project',
                                     help='The name of the repo where new branches will be pushed (e.g. llvm/llvm-project)')
release_workflow_parser.add_argument('sub_command', type=str, choices=['print-release-branch', 'auto', 'request-cherry-pick'],
                                     help='Print to stdout the name of the release branch ISSUE_NUMBER should be backported to')

request_cherry_pick_parser = subparsers.add_parser('request-cherry-pick')
request_cherry_pick_parser.add_argument('--issue-number', type=int,
                                        help='Backport the commits that fixed ISSUE_NUMBER')

llvmbot_git_config_parser = subparsers.add_parser('setup-llvmbot-git', help='Set the default user and email for the git repo in LLVM_PROJECT_DIR to llvmbot')

args = parser.parse_args()

if args.command == 'issue-subscriber':
    issue_subscriber = IssueSubscriber(args.token, args.repo, args.issue_number, args.label_name)
    issue_subscriber.run()
elif args.command == 'release-workflow':
    release_workflow = ReleaseWorkflow(args.token, args.repo, args.issue_number,
                                       args.branch_repo, args.branch_repo_token,
                                       args.llvm_project_dir)
    if args.sub_command == 'print-release-branch':
        release_workflow.print_release_branch()
    else:
        if not release_workflow.execute_command():
            sys.exit(1)
elif args.command == 'setup-llvmbot-git':
    setup_llvmbot_git()
elif args.command == 'request-cherry-pick':
    if args.issue_number:
        create_cherry_pick_request_from_closed_issue(args.issue_number, args.token)
    else:
        raise Exception("Not implemented")
