include "llvm/Option/OptParser.td"

// For options whose names are multiple letters, either one dash or
// two can precede the option name except those that start with 'o'.
class F<string name>: Flag<["--", "-"], name>;
class J<string name>: Joined<["--", "-"], name>;

multiclass Eq<string name, string help> {
  def NAME: Separate<["--", "-"], name>;
  def NAME # _eq: Joined<["--", "-"], name # "=">, Alias<!cast<Separate>(NAME)>,
    HelpText<help>;
}

multiclass B<string name, string help1, string help2> {
  def NAME: Flag<["--", "-"], name>, HelpText<help1>;
  def no_ # NAME: Flag<["--", "-"], "no-" # name>, HelpText<help2>;
}

defm auxiliary: Eq<"auxiliary", "Set DT_AUXILIARY field to the specified name">;

def Bsymbolic: F<"Bsymbolic">, HelpText<"Bind defined symbols locally">;

def Bsymbolic_functions: F<"Bsymbolic-functions">,
  HelpText<"Bind defined function symbols locally">;

def Bdynamic: F<"Bdynamic">, HelpText<"Link against shared libraries (default)">;

def Bstatic: F<"Bstatic">, HelpText<"Do not link against shared libraries">;

def build_id: F<"build-id">, HelpText<"Alias for --build-id=fast">;

def build_id_eq: J<"build-id=">, HelpText<"Generate build ID note">,
  MetaVarName<"[fast,md5,sha1,uuid,0x<hexstring>]">;

defm check_sections: B<"check-sections",
    "Check section addresses for overlaps (default)",
    "Do not check section addresses for overlaps">;

defm compress_debug_sections:
  Eq<"compress-debug-sections", "Compress DWARF debug sections">,
  MetaVarName<"[none,zlib]">;

defm defsym: Eq<"defsym", "Define a symbol alias">, MetaVarName<"<symbol>=<value>">;

defm split_stack_adjust_size
    : Eq<"split-stack-adjust-size",
         "Specify adjustment to stack size when a split-stack function calls a "
         "non-split-stack function">,
      MetaVarName<"<value>">;

defm library_path:
  Eq<"library-path", "Add a directory to the library search path">, MetaVarName<"<dir>">;

def O: JoinedOrSeparate<["-"], "O">, HelpText<"Optimize output file size">;

defm Tbss: Eq<"Tbss", "Same as --section-start with .bss as the sectionname">;

defm Tdata: Eq<"Tdata", "Same as --section-start with .data as the sectionname">;

defm Ttext: Eq<"Ttext", "Same as --section-start with .text as the sectionname">;

defm allow_multiple_definition: B<"allow-multiple-definition",
    "Allow multiple definitions",
    "Do not allow multiple definitions (default)">;

defm allow_shlib_undefined: B<"allow-shlib-undefined",
    "Allow unresolved references in shared libraries (default when linking a shared library)",
    "Do not allow unresolved references in shared libraries (default when linking an executable)">;

defm apply_dynamic_relocs: B<"apply-dynamic-relocs",
    "Apply link-time values for dynamic relocations",
    "Do not apply link-time values for dynamic relocations (default)">;

defm dependent_libraries: B<"dependent-libraries",
    "Process dependent library specifiers from input files (default)",
    "Ignore dependent library specifiers from input files">;

defm as_needed: B<"as-needed",
    "Only set DT_NEEDED for shared libraries if used",
    "Always set DT_NEEDED for shared libraries (default)">;

defm call_graph_ordering_file:
  Eq<"call-graph-ordering-file", "Layout sections to optimize the given callgraph">;

defm call_graph_profile_sort: B<"call-graph-profile-sort",
    "Reorder sections with call graph profile (default)",
    "Do not reorder sections with call graph profile">;

// -chroot doesn't have a help text because it is an internal option.
def chroot: Separate<["--", "-"], "chroot">;

def color_diagnostics: F<"color-diagnostics">,
  HelpText<"Alias for --color-diagnostics=always">;

def color_diagnostics_eq: J<"color-diagnostics=">,
  HelpText<"Use colors in diagnostics">,
  MetaVarName<"[auto,always,never]">;

defm cref: B<"cref",
    "Output cross reference table",
    "Do not output cross reference table">;

defm define_common: B<"define-common",
    "Assign space to common symbols",
    "Do not assign space to common symbols">;

defm demangle: B<"demangle",
    "Demangle symbol names (default)",
    "Do not demangle symbol names">;

def define_common: F<"define-common">,
  HelpText<"Assign space to common symbols">;

def disable_new_dtags: F<"disable-new-dtags">,
  HelpText<"Disable new dynamic tags">;

def discard_all: F<"discard-all">, HelpText<"Delete all local symbols">;

def discard_locals: F<"discard-locals">,
  HelpText<"Delete temporary local symbols">;

def discard_none: F<"discard-none">,
  HelpText<"Keep all symbols in the symbol table">;

defm dynamic_linker: Eq<"dynamic-linker", "Which dynamic linker to use">;

defm dynamic_list: Eq<"dynamic-list", "Read a list of dynamic symbols">;

defm eh_frame_hdr: B<"eh-frame-hdr",
    "Request creation of .eh_frame_hdr section and PT_GNU_EH_FRAME segment header",
    "Do not create .eh_frame_hdr section">;

def emit_relocs: F<"emit-relocs">, HelpText<"Generate relocations in output">;

def enable_new_dtags: F<"enable-new-dtags">,
  HelpText<"Enable new dynamic tags (default)">;

def end_group: F<"end-group">,
  HelpText<"Ignored for compatibility with GNU unless you pass --warn-backrefs">;

def end_lib: F<"end-lib">,
  HelpText<"End a grouping of objects that should be treated as if they were together in an archive">;

defm entry: Eq<"entry", "Name of entry point symbol">,
  MetaVarName<"<entry>">;

defm error_limit:
  Eq<"error-limit", "Maximum number of errors to emit before stopping (0 = no limit)">;

def error_unresolved_symbols: F<"error-unresolved-symbols">,
  HelpText<"Report unresolved symbols as errors">;

defm exclude_libs: Eq<"exclude-libs", "Exclude static libraries from automatic export">;

defm execute_only: B<"execute-only",
    "Mark executable sections unreadable",
    "Mark executable sections readable (default)">;

defm export_dynamic: B<"export-dynamic",
    "Put symbols in the dynamic symbol table",
    "Do not put symbols in the dynamic symbol table (default)">;

defm export_dynamic_symbol:
  Eq<"export-dynamic-symbol", "Put a symbol in the dynamic symbol table">;

defm fatal_warnings: B<"fatal-warnings",
    "Treat warnings as errors",
    "Do not treat warnings as errors (default)">;

defm filter: Eq<"filter", "Set DT_FILTER field to the specified name">;

defm fini: Eq<"fini", "Specify a finalizer function">, MetaVarName<"<symbol>">;

def fix_cortex_a53_843419: F<"fix-cortex-a53-843419">,
  HelpText<"Apply fixes for AArch64 Cortex-A53 erratum 843419">;

def fix_cortex_a8: F<"fix-cortex-a8">,
  HelpText<"Apply fixes for ARM Cortex-A8 erratum 657417">;

// This option is intentionally hidden from the user as the implementation
// is not complete.
def require_cet: F<"require-cet">;

def force_bti: F<"force-bti">,
  HelpText<"Force enable AArch64 BTI in PLT, warn if Input ELF file does not have GNU_PROPERTY_AARCH64_FEATURE_1_BTI property">;

defm format: Eq<"format", "Change the input format of the inputs following this option">,
  MetaVarName<"[default,elf,binary]">;

<<<<<<< HEAD
defm gc_sections: B<"gc-sections",
    "Enable garbage collection of unused sections",
    "Disable garbage collection of unused sections (default)">;
=======
def icf_all: F<"icf=all">, HelpText<"Enable identical code folding">;

def icf_none: F<"icf=none">, HelpText<"Disable identical code folding">;
>>>>>>> origin/release/5.x

defm gdb_index: B<"gdb-index",
    "Generate .gdb_index section",
    "Do not generate .gdb_index section (default)">;

defm gnu_unique: B<"gnu-unique",
  "Enable STB_GNU_UNIQUE symbol binding (default)",
  "Disable STB_GNU_UNIQUE symbol binding">;

defm hash_style: Eq<"hash-style", "Specify hash style (sysv, gnu or both)">;

def help: F<"help">, HelpText<"Print option help">;

def icf_all: F<"icf=all">, HelpText<"Enable identical code folding">;

def icf_safe: F<"icf=safe">, HelpText<"Enable safe identical code folding">;

def icf_none: F<"icf=none">, HelpText<"Disable identical code folding (default)">;

def ignore_function_address_equality: F<"ignore-function-address-equality">,
  HelpText<"lld can break the address equality of functions">;

def ignore_data_address_equality: F<"ignore-data-address-equality">,
  HelpText<"lld can break the address equality of data">;

defm image_base: Eq<"image-base", "Set the base address">;

defm init: Eq<"init", "Specify an initializer function">,
  MetaVarName<"<symbol>">;

defm just_symbols: Eq<"just-symbols", "Just link symbols">;

defm keep_unique: Eq<"keep-unique", "Do not fold this symbol during ICF">;

<<<<<<< HEAD
defm library: Eq<"library", "Root name of library to use">,
  MetaVarName<"<libName>">;
=======
def no_define_common: F<"no-define-common">,
  HelpText<"Do not assign space to common symbols">;

def no_demangle: F<"no-demangle">,
  HelpText<"Do not demangle symbol names">;
>>>>>>> origin/release/4.x

def m: JoinedOrSeparate<["-"], "m">, HelpText<"Set target emulation">;

defm Map: Eq<"Map", "Print a link map to the specified file">;

defm merge_exidx_entries: B<"merge-exidx-entries",
    "Enable merging .ARM.exidx entries (default)",
    "Disable merging .ARM.exidx entries">;

def nmagic: F<"nmagic">, MetaVarName<"<magic>">,
  HelpText<"Do not page align sections, link against static libraries.">;

def nostdlib: F<"nostdlib">,
  HelpText<"Only search directories specified on the command line">;

def no_color_diagnostics: F<"no-color-diagnostics">,
  HelpText<"Do not use colors in diagnostics">;

def no_dynamic_linker: F<"no-dynamic-linker">,
  HelpText<"Inhibit output of .interp section">;

def noinhibit_exec: F<"noinhibit-exec">,
  HelpText<"Retain the executable output file whenever it is still usable">;

def no_nmagic: F<"no-nmagic">, MetaVarName<"<magic>">,
  HelpText<"Page align sections (default)">;

def no_omagic: F<"no-omagic">, MetaVarName<"<magic>">,
  HelpText<"Do not set the text data sections to be writable, page align sections (default)">;

def no_rosegment: F<"no-rosegment">,
  HelpText<"Do not put read-only non-executable sections in their own segment">;

def no_undefined: F<"no-undefined">,
  HelpText<"Report unresolved symbols even if the linker is creating a shared library">;

def o: JoinedOrSeparate<["-"], "o">, MetaVarName<"<path>">,
  HelpText<"Path to file to write output">;

def oformat: Separate<["--"], "oformat">, MetaVarName<"<format>">,
  HelpText<"Specify the binary format for the output object file">;

def omagic: Flag<["--"], "omagic">, MetaVarName<"<magic>">,
  HelpText<"Set the text and data sections to be readable and writable, do not page align sections, link against static libraries">;

defm orphan_handling:
  Eq<"orphan-handling", "Control how orphan sections are handled when linker script used">;

defm pack_dyn_relocs:
  Eq<"pack-dyn-relocs", "Pack dynamic relocations in the given format">,
  MetaVarName<"[none,android,relr,android+relr]">;

def pac_plt: F<"pac-plt">,
  HelpText<"AArch64 only, use pointer authentication in PLT">;

defm use_android_relr_tags: B<"use-android-relr-tags",
    "Use SHT_ANDROID_RELR / DT_ANDROID_RELR* tags instead of SHT_RELR / DT_RELR*",
    "Use SHT_RELR / DT_RELR* tags (default)">;

def pic_veneer: F<"pic-veneer">,
  HelpText<"Always generate position independent thunks (veneers)">;

defm pie: B<"pie",
    "Create a position independent executable",
    "Do not create a position independent executable (default)">;

defm print_gc_sections: B<"print-gc-sections",
    "List removed unused sections",
    "Do not list removed unused sections (default)">;

defm print_icf_sections: B<"print-icf-sections",
    "List identical folded sections",
    "Do not list identical folded sections (default)">;

defm print_symbol_order: Eq<"print-symbol-order",
  "Print a symbol order specified by --call-graph-ordering-file into the speficied file">;

def pop_state: F<"pop-state">,
  HelpText<"Undo the effect of -push-state">;

def push_state: F<"push-state">,
  HelpText<"Save the current state of -as-needed, -static and -whole-archive">;

def print_map: F<"print-map">,
  HelpText<"Print a link map to the standard output">;

defm reproduce: Eq<"reproduce", "Write a tar file containing input files and command line options to reproduce link">;

defm rpath: Eq<"rpath", "Add a DT_RUNPATH to the output">;

def relocatable: F<"relocatable">, HelpText<"Create relocatable object file">;

defm retain_symbols_file:
  Eq<"retain-symbols-file", "Retain only the symbols listed in the file">,
  MetaVarName<"<file>">;

defm script: Eq<"script", "Read linker script">;

defm section_start: Eq<"section-start", "Set address of section">,
  MetaVarName<"<address>">;

def shared: F<"shared">, HelpText<"Build a shared object">;

defm soname: Eq<"soname", "Set DT_SONAME">;

defm sort_section:
  Eq<"sort-section", "Specifies sections sorting rule when linkerscript is used">;

def start_group: F<"start-group">,
  HelpText<"Ignored for compatibility with GNU unless you pass --warn-backrefs">;

def start_lib: F<"start-lib">,
  HelpText<"Start a grouping of objects that should be treated as if they were together in an archive">;

def strip_all: F<"strip-all">, HelpText<"Strip all symbols">;

def strip_debug: F<"strip-debug">, HelpText<"Strip debugging information">;

defm symbol_ordering_file:
  Eq<"symbol-ordering-file", "Layout sections to place symbols in the order specified by symbol ordering file">;

defm sysroot: Eq<"sysroot", "Set the system root">;

def target1_rel: F<"target1-rel">, HelpText<"Interpret R_ARM_TARGET1 as R_ARM_REL32">;

def target1_abs: F<"target1-abs">, HelpText<"Interpret R_ARM_TARGET1 as R_ARM_ABS32 (default)">;

defm target2:
  Eq<"target2", "Interpret R_ARM_TARGET2 as <type>, where <type> is one of rel, abs, or got-rel">,
  MetaVarName<"<type>">;

defm threads: B<"threads",
    "Run the linker multi-threaded (default)",
    "Do not run the linker multi-threaded">;

defm toc_optimize : B<"toc-optimize",
    "(PowerPC64) Enable TOC related optimizations (default)",
    "(PowerPC64) Disable TOC related optimizations">;

def trace: F<"trace">, HelpText<"Print the names of the input files">;

defm trace_symbol: Eq<"trace-symbol", "Trace references to symbols">;

defm undefined: Eq<"undefined", "Force undefined symbol during linking">,
  MetaVarName<"<symbol>">;

defm undefined_glob: Eq<"undefined-glob", "Force undefined symbol during linking">,
  MetaVarName<"<pattern>">;

defm unresolved_symbols:
  Eq<"unresolved-symbols", "Determine how to handle unresolved symbols">;

defm undefined_version: B<"undefined-version",
  "Allow unused version in version script (default)",
  "Report version scripts that refer undefined symbols">;

defm rsp_quoting: Eq<"rsp-quoting", "Quoting style for response files">,
  MetaVarName<"[posix,windows]">;

def v: Flag<["-"], "v">, HelpText<"Display the version number">;

def verbose: F<"verbose">, HelpText<"Verbose mode">;

def version: F<"version">, HelpText<"Display the version number and exit">;

defm version_script: Eq<"version-script", "Read a version script">;

defm warn_backrefs: B<"warn-backrefs",
    "Warn about backward symbol references to fetch archive members",
    "Do not warn about backward symbol references to fetch archive members (default)">;

defm warn_common: B<"warn-common",
    "Warn about duplicate common symbols",
    "Do not warn about duplicate common symbols (default)">;

defm warn_ifunc_textrel: B<"warn-ifunc-textrel",
    "Warn about using ifunc symbols with text relocations",
    "Do not warn about using ifunc symbols with text relocations (default)">;

defm warn_symbol_ordering: B<"warn-symbol-ordering",
    "Warn about problems with the symbol ordering file (default)",
    "Do not warn about problems with the symbol ordering file">;

def warn_unresolved_symbols: F<"warn-unresolved-symbols">,
  HelpText<"Report unresolved symbols as warnings">;

defm whole_archive: B<"whole-archive",
    "Force load of all members in a static library",
    "Do not force load of all members in a static library (default)">;

defm wrap: Eq<"wrap", "Use wrapper functions for symbol">,
  MetaVarName<"<symbol>=<symbol>">;

def z: JoinedOrSeparate<["-"], "z">, MetaVarName<"<option>">,
  HelpText<"Linker option extensions">;

<<<<<<< HEAD
def visual_studio_diagnostics_format : F<"vs-diagnostics">,
HelpText<"Format diagnostics for Visual Studio compatiblity">;

// Aliases
def: Separate<["-"], "f">, Alias<auxiliary>, HelpText<"Alias for --auxiliary">;
def: F<"call_shared">, Alias<Bdynamic>, HelpText<"Alias for --Bdynamic">;
def: F<"dy">, Alias<Bdynamic>, HelpText<"Alias for --Bdynamic">;
def: F<"dn">, Alias<Bstatic>, HelpText<"Alias for --Bstatic">;
def: F<"non_shared">, Alias<Bstatic>, HelpText<"Alias for --Bstatic">;
def: F<"static">, Alias<Bstatic>, HelpText<"Alias for --Bstatic">;
def: Flag<["-"], "d">, Alias<define_common>, HelpText<"Alias for --define-common">;
def: F<"dc">, Alias<define_common>, HelpText<"Alias for --define-common">;
def: F<"dp">, Alias<define_common>, HelpText<"Alias for --define-common">;
def: Flag<["-"], "x">, Alias<discard_all>, HelpText<"Alias for --discard-all">;
def: Flag<["-"], "X">, Alias<discard_locals>, HelpText<"Alias for --discard-locals">;
def: Flag<["-"], "q">, Alias<emit_relocs>, HelpText<"Alias for --emit-relocs">;
def: Flag<["-"], ")">, Alias<end_group>, HelpText<"Alias for --end-group">;
def: JoinedOrSeparate<["-"], "e">, Alias<entry>, HelpText<"Alias for --entry">;
def: Flag<["-"], "E">, Alias<export_dynamic>, HelpText<"Alias for --export-dynamic">;
def: Separate<["-"], "F">, Alias<filter>, HelpText<"Alias for --filter">;
def: Separate<["-"], "b">, Alias<format>, HelpText<"Alias for --format">;
def: JoinedOrSeparate<["-"], "l">, Alias<library>, HelpText<"Alias for --library">;
def: JoinedOrSeparate<["-"], "L">, Alias<library_path>, HelpText<"Alias for --library-path">;
def: F<"no-pic-executable">, Alias<no_pie>, HelpText<"Alias for --no-pie">;
def: Flag<["-"], "n">, Alias<nmagic>, HelpText<"Alias for --nmagic">;
def: Flag<["-"], "N">, Alias<omagic>, HelpText<"Alias for --omagic">;
def: Joined<["--"], "output=">, Alias<o>, HelpText<"Alias for -o">;
def: Separate<["--"], "output">, Alias<o>, HelpText<"Alias for -o">;
def: F<"pic-executable">, Alias<pie>, HelpText<"Alias for --pie">;
def: Flag<["-"], "M">, Alias<print_map>, HelpText<"Alias for --print-map">;
def: Flag<["-"], "r">, Alias<relocatable>, HelpText<"Alias for --relocatable">;
def: JoinedOrSeparate<["-"], "R">, Alias<rpath>, HelpText<"Alias for --rpath">;
def: JoinedOrSeparate<["-"], "T">, Alias<script>, HelpText<"Alias for --script">;
def: F<"Bshareable">, Alias<shared>, HelpText<"Alias for --shared">;
def: JoinedOrSeparate<["-"], "h">, Alias<soname>, HelpText<"Alias for --soname">;
def: Flag<["-"], "(">, Alias<start_group>, HelpText<"Alias for --start-group">;
def: Flag<["-"], "s">, Alias<strip_all>, HelpText<"Alias for --strip-all">;
def: Flag<["-"], "S">, Alias<strip_debug>, HelpText<"Alias for --strip-debug">;
def: Flag<["-"], "t">, Alias<trace>, HelpText<"Alias for --trace">;
def: JoinedOrSeparate<["-"], "y">, Alias<trace_symbol>, HelpText<"Alias for --trace-symbol">;
def: Separate<["-", "--"], "Ttext-segment">, Alias<Ttext>, HelpText<"Alias for --Ttext">;
def: Joined<["-", "--"], "Ttext-segment=">, Alias<Ttext>, HelpText<"Alias for --Ttext">;
def: JoinedOrSeparate<["-"], "u">, Alias<undefined>, HelpText<"Alias for --undefined">;
def: Flag<["-"], "V">, Alias<version>, HelpText<"Alias for --version">;
=======
// Aliases
def alias_auxiliary: Separate<["-"], "f">, Alias<auxiliary>;
def alias_Bdynamic_call_shared: F<"call_shared">, Alias<Bdynamic>;
def alias_Bdynamic_dy: F<"dy">, Alias<Bdynamic>;
def alias_Bstatic_dn: F<"dn">, Alias<Bstatic>;
def alias_Bstatic_non_shared: F<"non_shared">, Alias<Bstatic>;
def alias_Bstatic_static: F<"static">, Alias<Bstatic>;
def alias_L__library_path: J<"library-path=">, Alias<L>;
def alias_define_common_d: Flag<["-"], "d">, Alias<define_common>;
def alias_define_common_dc: F<"dc">, Alias<define_common>;
def alias_define_common_dp: F<"dp">, Alias<define_common>;
def alias_discard_all_x: Flag<["-"], "x">, Alias<discard_all>;
def alias_discard_locals_X: Flag<["-"], "X">, Alias<discard_locals>;
def alias_dynamic_list: J<"dynamic-list=">, Alias<dynamic_list>;
def alias_entry_e: JoinedOrSeparate<["-"], "e">, Alias<entry>;
def alias_entry_entry: J<"entry=">, Alias<entry>;
def alias_error_limit: J<"error-limit=">, Alias<error_limit>;
def alias_export_dynamic_E: Flag<["-"], "E">, Alias<export_dynamic>;
def alias_export_dynamic_symbol: J<"export-dynamic-symbol=">,
  Alias<export_dynamic_symbol>;
def alias_fini_fini: J<"fini=">, Alias<fini>;
def alias_format_b: S<"b">, Alias<format>;
def alias_hash_style_hash_style: J<"hash-style=">, Alias<hash_style>;
def alias_init_init: J<"init=">, Alias<init>;
def alias_l__library: J<"library=">, Alias<l>;
def alias_omagic: Flag<["-"], "N">, Alias<omagic>;
def alias_o_output: Joined<["--"], "output=">, Alias<o>;
def alias_o_output2 : Separate<["--"], "output">, Alias<o>;
def alias_pie_pic_executable: F<"pic-executable">, Alias<pie>;
def alias_relocatable_r: Flag<["-"], "r">, Alias<relocatable>;
def alias_retain_symbols_file: S<"retain-symbols-file">, Alias<retain_symbols_file>;
def alias_rpath_R: JoinedOrSeparate<["-"], "R">, Alias<rpath>;
def alias_rpath_rpath: J<"rpath=">, Alias<rpath>;
def alias_script_T: JoinedOrSeparate<["-"], "T">, Alias<script>;
def alias_shared_Bshareable: F<"Bshareable">, Alias<shared>;
def alias_soname_h: JoinedOrSeparate<["-"], "h">, Alias<soname>;
def alias_soname_soname: S<"soname">, Alias<soname>;
def alias_sort_section: J<"sort-section=">, Alias<sort_section>;
def alias_script: J<"script=">, Alias<script>;
def alias_strip_all: Flag<["-"], "s">, Alias<strip_all>;
def alias_strip_debug_S: Flag<["-"], "S">, Alias<strip_debug>;
def alias_Tbss: J<"Tbss=">, Alias<Tbss>;
def alias_Tdata: J<"Tdata=">, Alias<Tdata>;
def alias_trace: Flag<["-"], "t">, Alias<trace>;
def alias_trace_symbol_y : JoinedOrSeparate<["-"], "y">, Alias<trace_symbol>;
def alias_Ttext: J<"Ttext=">, Alias<Ttext>;
def alias_Ttext_segment: S<"Ttext-segment">, Alias<Ttext>;
def alias_Ttext_segment_eq: J<"Ttext-segment=">, Alias<Ttext>;
def alias_undefined_eq: J<"undefined=">, Alias<undefined>;
def alias_undefined_u: JoinedOrSeparate<["-"], "u">, Alias<undefined>;
def alias_version_V: Flag<["-"], "V">, Alias<version>;
def alias_wrap_wrap: J<"wrap=">, Alias<wrap>;

// Our symbol resolution algorithm handles symbols in archive files differently
// than traditional linkers, so we don't need --start-group and --end-group.
// These options are recongized for compatibility but ignored.
def end_group: F<"end-group">;
def end_group_paren: Flag<["-"], ")">;
def start_group: F<"start-group">;
def start_group_paren: Flag<["-"], "(">;

// Ignore LTO plugin-related options.
// clang -flto passes -plugin and -plugin-opt to the linker. This is required
// for ld.gold and ld.bfd to get LTO working. But it's not for lld which doesn't
// rely on a plugin. Instead of detecting which linker is used on clang side we
// just ignore the option on lld side as it's easier. In fact, the linker could
// be called 'ld' and understanding which linker is used would require parsing of
// --version output.
def plugin: S<"plugin">;
def plugin_eq: J<"plugin=">;
def plugin_opt: S<"plugin-opt">;
def plugin_opt_eq: J<"plugin-opt=">;

// Options listed below are silently ignored for now for compatibility.
def allow_shlib_undefined: F<"allow-shlib-undefined">;
def cref: Flag<["--"], "cref">;
def demangle: F<"demangle">;
def detect_odr_violations: F<"detect-odr-violations">;
def g: Flag<["-"], "g">;
def M: Flag<["-"], "M">;
def Map: JS<"Map">;
def no_add_needed: F<"no-add-needed">;
def no_allow_shlib_undefined: F<"no-allow-shlib-undefined">;
def no_copy_dt_needed_entries: F<"no-copy-dt-needed-entries">,
  Alias<no_add_needed>;
def no_dynamic_linker: F<"no-dynamic-linker">;
def no_fatal_warnings: F<"no-fatal-warnings">;
def no_mmap_output_file: F<"no-mmap-output-file">;
def no_warn_common: F<"no-warn-common">;
def no_warn_mismatch: F<"no-warn-mismatch">;
def rpath_link: S<"rpath-link">;
def rpath_link_eq: J<"rpath-link=">;
def sort_common: F<"sort-common">;
def stats: F<"stats">;
def warn_execstack: F<"warn-execstack">;
def warn_shared_textrel: F<"warn-shared-textrel">;
def EB : F<"EB">;
def EL : F<"EL">;
def G: JoinedOrSeparate<["-"], "G">;
def Qy : F<"Qy">;

// Aliases for ignored options
def alias_Map_eq: J<"Map=">, Alias<Map>;
def alias_version_script_version_script: J<"version-script=">,
  Alias<version_script>;
>>>>>>> origin/release/4.x

// LTO-related options.
def lto_aa_pipeline: J<"lto-aa-pipeline=">,
  HelpText<"AA pipeline to run during LTO. Used in conjunction with -lto-newpm-passes">;
def lto_debug_pass_manager: F<"lto-debug-pass-manager">,
  HelpText<"Debug new pass manager">;
def lto_new_pass_manager: F<"lto-new-pass-manager">,
  HelpText<"Use new pass manager">;
def lto_newpm_passes: J<"lto-newpm-passes=">,
  HelpText<"Passes to run during LTO">;
def lto_O: J<"lto-O">, MetaVarName<"<opt-level>">,
  HelpText<"Optimization level for LTO">;
def lto_partitions: J<"lto-partitions=">,
  HelpText<"Number of LTO codegen partitions">;
def lto_cs_profile_generate: F<"lto-cs-profile-generate">,
  HelpText<"Perform context senstive PGO instrumentation">;
def lto_cs_profile_file: J<"lto-cs-profile-file=">,
  HelpText<"Context sensitive profile file path">;
def lto_obj_path_eq: J<"lto-obj-path=">;
def lto_sample_profile: J<"lto-sample-profile=">,
  HelpText<"Sample profile file path">;
def disable_verify: F<"disable-verify">;
defm mllvm: Eq<"mllvm", "Additional arguments to forward to LLVM's option processing">;
def opt_remarks_filename: Separate<["--"], "opt-remarks-filename">,
  HelpText<"YAML output file for optimization remarks">;
def opt_remarks_passes: Separate<["--"], "opt-remarks-passes">,
  HelpText<"Regex for the passes that need to be serialized to the output file">;
def opt_remarks_with_hotness: Flag<["--"], "opt-remarks-with-hotness">,
  HelpText<"Include hotness information in the optimization remarks file">;
def opt_remarks_format: Separate<["--"], "opt-remarks-format">,
  HelpText<"The format used for serializing remarks (default: YAML)">;
defm plugin_opt: Eq<"plugin-opt", "specifies LTO options for compatibility with GNU linkers">;
def save_temps: F<"save-temps">;
def thinlto_cache_dir: J<"thinlto-cache-dir=">,
  HelpText<"Path to ThinLTO cached object file directory">;
defm thinlto_cache_policy: Eq<"thinlto-cache-policy", "Pruning policy for the ThinLTO cache">;
def thinlto_emit_imports_files: F<"thinlto-emit-imports-files">;
def thinlto_index_only: F<"thinlto-index-only">;
def thinlto_index_only_eq: J<"thinlto-index-only=">;
def thinlto_jobs: J<"thinlto-jobs=">, HelpText<"Number of ThinLTO jobs">;
def thinlto_object_suffix_replace_eq: J<"thinlto-object-suffix-replace=">;
def thinlto_prefix_replace_eq: J<"thinlto-prefix-replace=">;

def: J<"plugin-opt=O">, Alias<lto_O>, HelpText<"Alias for -lto-O">;
def: F<"plugin-opt=debug-pass-manager">,
  Alias<lto_debug_pass_manager>, HelpText<"Alias for -lto-debug-pass-manager">;
def: F<"plugin-opt=disable-verify">, Alias<disable_verify>, HelpText<"Alias for -disable-verify">;
def plugin_opt_dwo_dir_eq: J<"plugin-opt=dwo_dir=">,
  HelpText<"Directory to store .dwo files when LTO and debug fission are used">;
def plugin_opt_emit_llvm: F<"plugin-opt=emit-llvm">;
def: J<"plugin-opt=jobs=">, Alias<thinlto_jobs>, HelpText<"Alias for -thinlto-jobs">;
def: J<"plugin-opt=lto-partitions=">, Alias<lto_partitions>, HelpText<"Alias for -lto-partitions">;
def plugin_opt_mcpu_eq: J<"plugin-opt=mcpu=">;
def: F<"plugin-opt=new-pass-manager">,
  Alias<lto_new_pass_manager>, HelpText<"Alias for -lto-new-pass-manager">;
def: F<"plugin-opt=cs-profile-generate">,
  Alias<lto_cs_profile_generate>, HelpText<"Alias for -lto-cs-profile-generate">;
def: J<"plugin-opt=cs-profile-path=">,
  Alias<lto_cs_profile_file>, HelpText<"Alias for -lto-cs-profile-file">;
def: J<"plugin-opt=obj-path=">,
  Alias<lto_obj_path_eq>,
  HelpText<"Alias for -lto-obj-path=">;
def: J<"plugin-opt=sample-profile=">,
  Alias<lto_sample_profile>, HelpText<"Alias for -lto-sample-profile">;
def: F<"plugin-opt=save-temps">, Alias<save_temps>, HelpText<"Alias for -save-temps">;
def: F<"plugin-opt=thinlto-emit-imports-files">,
  Alias<thinlto_emit_imports_files>,
  HelpText<"Alias for -thinlto-emit-imports-files">;
def: F<"plugin-opt=thinlto-index-only">,
  Alias<thinlto_index_only>,
  HelpText<"Alias for -thinlto-index-only">;
def: J<"plugin-opt=thinlto-index-only=">,
  Alias<thinlto_index_only_eq>,
  HelpText<"Alias for -thinlto-index-only=">;
def: J<"plugin-opt=thinlto-object-suffix-replace=">,
 Alias<thinlto_object_suffix_replace_eq>,
 HelpText<"Alias for -thinlto-object-suffix-replace=">;
def: J<"plugin-opt=thinlto-prefix-replace=">,
  Alias<thinlto_prefix_replace_eq>,
  HelpText<"Alias for -thinlto-prefix-replace=">;

// Ignore LTO plugin-related options.
// clang -flto passes -plugin and -plugin-opt to the linker. This is required
// for ld.gold and ld.bfd to get LTO working. But it's not for lld which doesn't
// rely on a plugin. Instead of detecting which linker is used on clang side we
// just ignore the option on lld side as it's easier. In fact, the linker could
// be called 'ld' and understanding which linker is used would require parsing of
// --version output.
defm plugin: Eq<"plugin", "Ignored for compatibility with GNU linkers">;

def plugin_opt_fresolution_eq: J<"plugin-opt=-fresolution=">;
def plugin_opt_pass_through_eq: J<"plugin-opt=-pass-through=">;
def plugin_opt_thinlto: J<"plugin-opt=thinlto">;
def plugin_opt_slash: J<"plugin-opt=/">;

// Options listed below are silently ignored for now for compatibility.
def: F<"detect-odr-violations">;
def: Flag<["-"], "g">;
def: F<"long-plt">;
def: F<"no-add-needed">;
def: F<"no-copy-dt-needed-entries">;
def: F<"no-ctors-in-init-array">;
def: F<"no-keep-memory">;
def: F<"no-mmap-output-file">;
def: F<"no-pipeline-knowledge">;
def: F<"no-warn-mismatch">;
def: Flag<["-"], "p">;
def: Separate<["--", "-"], "rpath-link">;
def: J<"rpath-link=">;
def: F<"secure-plt">;
def: F<"sort-common">;
def: F<"stats">;
def: F<"warn-execstack">;
def: F<"warn-once">;
def: F<"warn-shared-textrel">;
def: F<"EB">;
def: F<"EL">;
def: JoinedOrSeparate<["-"], "G">;
def: F<"Qy">;

// Hidden option used for testing MIPS multi-GOT implementation.
defm mips_got_size:
  Eq<"mips-got-size", "Max size of a single MIPS GOT. 0x10000 by default.">,
  Flags<[HelpHidden]>;
