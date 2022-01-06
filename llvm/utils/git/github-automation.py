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
import sys



class IssueSubscriber:

    def __init__(self, args):
        self.repo = github.Github(args.token).get_repo(args.repo)
        self.org = github.Github(args.token).get_organization(self.repo.organization.login)
        self.issue = self.repo.get_issue(args.issue_number)
        self.team_name = 'issue-subscribers-{}'.format(args.label_name).lower()

    def run(self):
        for team in self.org.get_teams():
            if self.team_name != team.name.lower():
                continue
            comment = '@llvm/{}'.format(team.slug)
            self.issue.create_comment(comment)
            return True
        return False

def setup_llvmbot_git(git_dir = '.'):
    repo = Repo(git_dir)
    with repo.config_writer() as config:
        config.set_value('user', 'name', 'llvmbot')
        config.set_value('user', 'email', 'llvmbot@llvm.org')

class ReleaseWorkflow:

    def __init__(self, args):
        self.args = args
        self.token = args.token
        if args.branch_repo_token:
            self.branch_repo_token = args.branch_repo_token
        else:
            print("assigning branch repo token to {}".format(self.token))
            self.branch_repo_token = self.token

        self.repo_name = args.repo
        self.branch_repo_name = args.branch_repo

    def get_issue_number(self):
        return self.args.issue_number

    def get_issue(self):
        repo = github.Github(self.token).get_repo(self.repo_name)
        issue = repo.get_issue(self.get_issue_number())
        return issue

    def get_push_url(self):
        return 'https://{}@github.com/{}'.format(self.branch_repo_token, self.branch_repo_name)

    def get_branch_name(self):
        return 'issue{}'.format(self.get_issue_number())

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

    def issue_notify_cherry_pick_failure(self, commit):
        message = "Failed to cherry-pick: {}".format(commit)
        if os.getenv('CI'):
            actions_url = 'https://github.com/{}/actions/runs/{}'.format(os.getenv('GITHUB_REPOSITORY'), os.getenv('GITHUB_RUN_ID'))
            message += actions_url
        issue = self.get_issue()
        issue.create_comment(message)
        issue.add_to_labels('release:cherry-pick-failed')

    def create_branch(self, commits):
        print('cherry-picking', commits)
        local_repo = Repo(self.args.local_source_dir)
        for c in commits:
            if not local_repo.git.cherry_pick('-x', c):
                self.issue_notify_cherry_pick_failure(c)
                return False

        branch_name = self.get_branch_name()
        push_url = self.get_push_url()
        print('Pushing to {} {}'.format(push_url, branch_name))
        local_repo.git.push(push_url, 'HEAD:{}'.format(branch_name))

        self.issue_notify_branch()

    def create_pull_request(branch):
        repo = Github(self.token).get_repo(self.branch_repo_name)
        issue_ref = '{}#{}'.format(self.repo_name, self.get_issue_number())
        pull = repo.create_pull(title='PR for {}'.format(issue_ref),
                                body='resolves {}'.format(issue_ref),
                                base=self.get_release_branch(),
                                head='{}:{}'.format(repo.owner, self.get_branch_name()),
                                maintainer_can_modify=False)
        self.issue_notify_pull_request()
        

    def execute_command(self):
        for line in sys.stdin:
            line.rstrip()
            m = re.search("/([a-z-]+)\s(.+)", line)
            print(m)
            if not m:
                continue
            command = m.group(1)
            args = m.group(2)

            if command == 'cherry-pick':
                self.create_branch(args.split())
                return True

            if command == 'branch':
                self.create_pull_request(args)
                return True
        
        return False

def get_default_repo():
    default = os.getenv('GITHUB_REPOSITORY')
    if not default:
        default = 'llvm/llvm-project'
    return default


parser = argparse.ArgumentParser()
parser.add_argument('--token', type=str, required=True)
parser.add_argument('--repo', type=str, default=get_default_repo())
subparsers = parser.add_subparsers(dest='command')

issue_subscriber_parser = subparsers.add_parser('issue-subscriber')
issue_subscriber_parser.add_argument('--label-name', type=str, required=True)
issue_subscriber_parser.add_argument('--issue-number', type=int, required=True)

release_workflow_parser = subparsers.add_parser('release-workflow')
release_workflow_parser.add_argument('--issue-number', type=int, required=True)
release_workflow_parser.add_argument('--branch-repo-token', type=str)
release_workflow_parser.add_argument('--repo', type=str, default=get_default_repo())
release_workflow_parser.add_argument('--branch-repo', type=str, default='llvmbot/llvm-project')
release_workflow_parser.add_argument('--local-source-dir', type=str, default='.')
release_workflow_parser.add_argument('sub_command', type=str, choices=['print-release-branch', 'auto'])

llvmbot_git_config_parser = subparsers.add_parser('setup-llvmbot-git')

args = parser.parse_args()
print(args)

if args.command == 'issue-subscriber':
    issue_subscriber = IssueSubscriber(args)
    issue_subscriber.run()
elif args.command == 'release-workflow':
    release_workflow = ReleaseWorkflow(args)
    if args.sub_command == 'print-release-branch':
        release_workflow.print_release_branch()
    else:
        release_workflow.execute_command()
elif args.command == 'setup-llvmbot-git':
    setup_llvmbot_git()
