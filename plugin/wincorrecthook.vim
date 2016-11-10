scriptencoding utf-8

if exists('g:loaded_wincorrecthook_vim')
    finish
endif
let g:loaded_wincorrecthook_vim = 1

let s:save_cpo = &cpo
set cpo&vim

" -----
let s:arch=''
if has('win64')
  let s:arch='x64'
elseif has('win32')
  "let s:arch='x86'
  echohl WarningMsg
  echo "wincorrecthook-vim\nSorry, MS-Windows 32bit version is not support"
  echohl None
else
  echohl WarningMsg
  echo "wincorrecthook-vim\nSorry, this platform is not support"
  echohl None
end

if s:arch != ''
  call wincorrecthook#hook(expand('<sfile>:p:h').'\..\autoload', s:arch)
endif
" -----

let &cpo = s:save_cpo
unlet s:save_cpo
