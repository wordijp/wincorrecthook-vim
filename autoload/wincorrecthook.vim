function! wincorrecthook#hook(dlldir, arch)
  call libcall(a:dlldir.'\injector_'.a:arch, 'hookStart', a:dlldir.'\hook_'.a:arch.'.dll;'.getpid())
endfunction
