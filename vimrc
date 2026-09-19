set loadplugins
let g:loaded_gitgutter = 1
" Wrap vim-plug in a guard so it only loads once per session
if !exists('g:loaded_my_plugins')
  let g:loaded_my_plugins = 1

  call plug#begin('~/.vim/plugged')
  Plug 'vim-airline/vim-airline'
  Plug 'vim-airline/vim-airline-themes'
  Plug 'junegunn/fzf', { 'do': { -> fzf#install() } }
  Plug 'junegunn/fzf.vim'
  Plug 'neoclide/coc.nvim', {'branch': 'release'}
  Plug 'vim-scripts/taglist.vim'
  call plug#end()
endif

filetype plugin indent on

set t_Co=256
let g:airline_theme='dark'
syntax on
set background=dark
set nocompatible
set ruler
set laststatus=2
set incsearch
set hlsearch
set ai
set nobackup
set mouse=a
set tabstop=4
set shiftwidth=4
set expandtab
set number
set encoding=utf-8
set splitright
" set smartindent
set copyindent
set pastetoggle=~~
set bs=2
set writebackup
set showcmd
set showmatch
se ic
" Turn on line highlighting under the cursor
set cursorline
" Customize cursorline: Bold text with a distinct background color
highlight CursorLine cterm=bold ctermbg=236 guibg=#303030
" Make the active line number bold and bright yellow
set norelativenumber
highlight CursorLineNr cterm=bold ctermfg=yellow guifg=yellow

"clears highlighting
map <C-J> :nohl<CR>

"Comment for toggle
let s:comment_map = {
	\   "c": '\/\/',
	\   "cpp": '\/\/',
	\   "go": '\/\/',
	\   "java": '\/\/',
	\   "javascript": '\/\/',
	\   "lua": '--',
	\   "scala": '\/\/',
	\   "php": '\/\/',
	\   "python": '#',
	\   "ruby": '#',
	\   "rust": '\/\/',
	\   "sh": '#',
	\   "desktop": '#',
	\   "fstab": '#',
	\   "conf": '#',
	\   "profile": '#',
	\   "bashrc": '#',
	\   "bash_profile": '#',
	\   "mail": '>',
	\   "eml": '>',
	\   "bat": 'REM',
	\   "ahk": ';',
	\   "vim": '"',
	\   "tex": '%',
	\ }
function! ToggleComment()
    if has_key(s:comment_map, &filetype)
        let comment_leader = s:comment_map[&filetype]
        if getline('.') =~ "^\\s*" . comment_leader . " "
            " Uncomment the line
            execute "silent s/^\\(\\s*\\)" . comment_leader . " /\\1/"
        else
            if getline('.') =~ "^\\s*" . comment_leader
                " Uncomment the line
                execute "silent s/^\\(\\s*\\)" . comment_leader . "/\\1/"
            else
                " Comment the line
                execute "silent s/^\\(\\s*\\)/\\1" . comment_leader . " /"
            end
        end
    else
        echo "No comment leader found for filetype"
    end
endfunction
nnoremap <leader><Space> :call ToggleComment()<cr>
vnoremap <leader><Space> :call ToggleComment()<cr>

" Auto-format C/C++ files on save using your explicit global config
" autocmd BufWritePre *.c,*.cpp,*.h,*.hpp execute "%!clang-format -style=file:/home/rasp/.clang-format"
" Use native Vim C-indenting for '=' (handles partial selections perfectly)
set cinoptions+=p0,g0,(0,{1s,f0,t0
autocmd FileType c,cpp setlocal equalprg=
autocmd FileType c,cpp setlocal cindent
" Format current file or selection with \cf (or <leader>cf)
nnoremap <leader>cf :%!clang-format -style=file:/home/rasp/.clang-format<CR>
vnoremap <leader>cf :%!clang-format -style=file:/home/rasp/.clang-format<CR>
" Alternative: Format using Ctrl + K
nnoremap <C-K> :%!clang-format -style=file:/home/rasp/.clang-format<CR>
vnoremap <C-K> :%!clang-format -style=file:/home/rasp/.clang-format<CR>

" Use Tab to navigate the completion menu
inoremap <expr> <TAB> coc#pum#visible() ? coc#pum#next(1) : "\<TAB>"
inoremap <expr> <S-TAB> coc#pum#visible() ? coc#pum#prev(1) : "\<C-h>"
" Use Enter to confirm completion choice
inoremap <expr> <CR> coc#pum#visible() ? coc#pum#confirm() : "\<CR>"
" Go to definition (gd) and view documentation (K)
nmap <silent> gd <Plug>(coc-definition)
nmap <silent> gy <Plug>(coc-type-definition)
nmap <silent> gi <Plug>(coc-implementation)
nmap <silent> gr <Plug>(coc-references)
nnoremap <silent> K :call ShowDocumentation()<CR>
function! ShowDocumentation()
  if CocAction('hasProvider', 'hover')
    call CocActionAsync('doHover')
  endif
endfunction

" CSCOPE Related
" Press Leader + cr to rebuild database and refresh cscope connection
nnoremap <leader>cr :!rm -f cscope.out cscope.in.out cscope.po.out && cscope -b -R -q<CR>:cs reset<CR>
" Automatically search for 'tags' file starting from current dir up to root
set tags=./tags;,tags

" ==============================================================================
" Cscope & Ctags Code Navigation Configuration
" ==============================================================================
if has("cscope")
" ==============================================================================
" Native Interactive Cscope Configuration (No Quickfix Window)
" ==============================================================================
    " 1. Search Settings
    set csprg=/usr/bin/cscope
    set csto=0                  " Search Cscope database before Ctags (0 = cscope first)
    set cst                     " Use Cscope for tag search operations
    set nocsverb                " Suppress status messages while adding database

    " Disable Quickfix output for all query types
    set cscopequickfix=""

    " 2. Automatic Database Loading
    if filereadable("cscope.out")
        cs add cscope.out
    endif
    " 3. Clean Mappings
    nmap <C-_> :cs find c <cword><CR>
    nmap <C-s> :cs find s <cword><CR>
    nmap g<C-]> :tag <cword><CR>
    nmap <C-e> :cs find e <cword><CR>
    nmap <C-f> :cs find f <cword><CR>
    nmap <C-a> :cs find a <cword><CR>
endif

" --- Navigation Shortcuts ---
" Ctags: Jump to definition with Ctrl+] and back with Ctrl+t
nnoremap <C-]> :ta <C-r><C-w><CR>
" Cscope Shortcuts (<leader> = \)
" Find symbol / function references
nnoremap <leader>cs :cs find s <C-r><C-w><CR>
" Find function definition
nnoremap <leader>cg :cs find g <C-r><C-w><CR>
" Find functions calling this function
nnoremap <leader>cc :cs find c <C-r><C-w><CR>
" Find text string
nnoremap <leader>ct :cs find t <C-r><C-w><CR>

augroup VimWindowDimming
  autocmd!
  " Active window: keep cursorline on and restore syntax highlighting
  autocmd WinEnter,BufWinEnter * setlocal cursorline syntax=on
  " Inactive window: disable cursorline and turn off syntax highlighting
  autocmd WinLeave * setlocal nocursorline syntax=off
augroup END

" --- Compile And Run ---
" F5: Compile with C99/C90 safety warnings
nnoremap <F5> :w<CR>:!gcc -Wall -Wextra -std=c99 % -o %< <CR>
" F6: Compile and execute in a vertical terminal split on the right
nnoremap <F6> :w<CR>:botright vert term ./%:r<CR>
" Load Termdebug visual layout
packadd! termdebug
let g:termdebug_wide = 1
" F9: Compile silently, swallow verbose syntax messages, and launch Termdebug
" Search files in current project directory
nnoremap <C-p> :Files<CR>
" nnoremap <F9> :w<CR>:silent !gcc -Wall -Wextra -std=c99 -g % -o %<<CR>:silent! Termdebug ./%:r<CR>
" Synchronous compile with error checking before starting Termdebug
" " Fold based on indentation levels instead of syntax rules
autocmd FileType c,cpp setlocal foldmethod=indent | setlocal foldlevel=99
nnoremap <F9> :w<CR>:silent !gcc -Wall -Wextra -std=c99 -g % -o %:r<CR>:redraw!<CR>:if executable('./' . expand('%:r')) \| execute 'Termdebug ./' . expand('%:r') \| else \| echo "Compilation failed!" \| endif<CR>
" ==============================================================================
" FZF + Cscope with Search Term & Line Highlighting
" ==============================================================================
nnoremap <leader>fi :FZF<CR>
function! FzfCscope(query, type)
  let l:db = findfile("cscope.out", ".;")
  if empty(l:db)
    echohl WarningMsg | echo "cscope.out not found!" | echohl None
    return
  endif

  let l:db_path = fnamemodify(l:db, ":p")
  let l:dir = fnamemodify(l:db_path, ":h")
  let l:cmd = 'cscope -d -f ' . shellescape(l:db_path) . ' -L -' . a:type . ' ' . shellescape(a:query)

  " Dynamic preview command: extracts file ($1) & line ($3), extracts context, and highlights query string
  let l:preview = 'LINE=$(echo {} | awk "{print \$3}"); if [ -z "$LINE" ]; then LINE=1; fi; FILE=$(echo {} | awk "{print \$1}"); sed -n "$((LINE>5?LINE-5:1)),$((LINE+10))p" "$FILE" | grep --color=always -i -C 10 ' . shellescape(a:query)

  call fzf#run(fzf#wrap({
        \ 'source': l:cmd,
        \ 'dir': l:dir,
        \ 'sink': function('FzfCscopeHandler'),
        \ 'options': [
        \   '--prompt', 'Cscope (' . a:query . ')> ',
        \   '--preview', l:preview,
        \   '--preview-window', 'right:50%'
        \ ]
        \ }))
endfunction

function! FzfCscopeHandler(line)
  let l:parts = split(a:line)
  if len(l:parts) >= 3
      " Push current position to Vim's tag stack before navigating so Ctrl+T works
     let l:pos = [bufnr('%'), line('.'), col('.'), 0]
     let l:item = {'bufnr': l:pos[0], 'from': l:pos, 'tagname': expand('<cword>')}
     let l:winid = win_getid()
     call settagstack(l:winid, {'items': [l:item]}, 'a')

    execute 'e +' . l:parts[2] . ' ' . fnameescape(l:parts[0])
  endif
endfunction
" Key Mappings
nnoremap <C-\>s :call FzfCscope(expand('<cword>'), '0')<CR>
nnoremap <C-\>g :call FzfCscope(expand('<cword>'), '1')<CR>
nnoremap <C-\>d :call FzfCscope(expand('<cword>'), '2')<CR>
nnoremap <C-\>c :call FzfCscope(expand('<cword>'), '3')<CR>
nnoremap <C-\>t :call FzfCscope(expand('<cword>'), '4')<CR>
nnoremap <C-\>e :call FzfCscope(expand('<cword>'), '6')<CR>
nnoremap <C-\>f :call FzfCscope(expand('<cword>'), '7')<CR>
nnoremap <C-\>i :call FzfCscope(expand('<cword>'), '8')<CR>
nnoremap <C-b> :Buffers<CR>
nnoremap <leader>bd :bprevious \| bdelete #<CR>
