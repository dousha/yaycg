yaycg - Yet another yaml configuration generator
================================================

Tired of copying template file for your Minecraft/Bukkit/Spigot/WhateverYouUse
plugins? Annoyed by not-programmer-friendly syntax of YAML? Just want to fill
in the blank instead of copying same piece of code then modifying them? Well,
here's a simple solution.

Usage
-----

This program is standalone. No other library needed, even YAML libraries are
not used here. To build this program, simply type the command below.
`g++ -std=c++11 <main_file_you_downloaded> -o <executable_name_you_want>`

Or, just `make && make run`. No installation needed.

The YAML configurations would be saved as `config.yml`. If the file exists, it
would be overwritten.

Editing Modes
-------------

There are 3 editing modes, the normal, the enum mode, and the list mode.

**Normal mode**
In this mode, what you type is what you fill. Press <Enter> without input
anything means use it's default value.

**Enum mode**
In this mode, a list of enumrations would be shown. You can select an element
by typing it's number. You can't type string in enum editing mode, even the 
string you type is exactly the element you want, yet. You can't use default
value in enum selecting mode neither, yet.

**List mode**
In this mode, what you type would be saved as a YAML list. You don't need to
type dashes or spaces. Type `###` to save and continue editing. Type `^^^` to
edit last line. By doing this, the last line will be covered by the new one.
If you leave it blank, a blank term would be generated. If you want to delete
the last line you can type `___` (3 underscores). Any other input will be saved
as a term of the list. You can't use enum in list editing mode, yet.

Template File Syntax
--------------------

This program is not a wizard. It needs a template file in order to generate
the configuration file you want. By default, the `template.txt` is used as
the template. You can specify your template file name by passing an argument.

The syntax is same as the original YAML syntax, but some modifies are made.

If the content below are confusing, please skip it and turn to the
`template.txt` in the repository. Sorry for that.

-------------------------------------------------------------------------------

A template file are divided into 2 parts by `----------` (10 dashes). The
former part is *template area* and the later part is *enumration area*. This
two area cannot be mixed and if there's not exactly 10 dashes, it would not be
recognized by the program.

**Basic Rules**

0. You have to use Spaces instead of Tabs. And that's YAML asked us to do, too.
1. To generate a ID here (the continuous `int`s used by plugins), simply write
`$id`
2. The default values are written in YAML style (`key:value` means by default,
`key` has the value `value`)
3. To generate a list, write `LIST` behind the colon (e.g. `listy:LIST`)
4. To use enumrations, use `%x` where `x` is an `int`. Then declare it in the
enumration area like the example below.

```
enumy:%1
----------
%1
ENUM_ELEMENT_1
ENUM_ELEMENT_2
OR_ANYTHING_YOU_LIKE
NOT_TAHT_SPACES_ARE_NOT_RECOMMENDED
lowercase_characters_are_also_not_recommended
```
