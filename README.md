# wincorrecthook-vim

vim plugin for Windows API hook, because fix the difference with the Linux version

## Support Version

current is MS-Windows 64 bit version only

## Install

```vim:.vimrc
NeoBundle 'wordijp/wincorrecthook-vim'
```

please restart vim

if rebuild from source code

1. run build_x64.bat(need for Microsoft Visual Studio, default is 2015(VC++14) x64 version)

## Usage

```vim:
:echo exepath('where')
C:/Windows/system32/where.EXE
# hook before
# C:\Windows\system32\where.EXE
```

## Licence

MIT
