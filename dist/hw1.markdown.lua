local ran_ok, error = pcall(function() local kpse = require("kpse") kpse.set_program_name("luatex") local lfs = require("lfs") local cacheDir = "./_markdown_hw1" if not lfs.isdir(cacheDir) then assert(lfs.mkdir(cacheDir)) end local md = require("markdown") local convert = md.new({cacheDir = "./_markdown_hw1", frozenCacheFileName = "./_markdown_hw1/frozenCache.tex", frozenCacheCounter = 0, } ) local file = assert(io.open("./hw1.markdown.in", "r")) local input = assert(file:read("*a")) assert(file:close()) print(convert(input:gsub("\r\n?", "\n") .. "\n")) end) if not ran_ok then local file = io.open("./hw1.markdown.err", "w") if file then file:write(error .. "\n") file:close() end print('\\markdownError{An error was encountered while executing Lua code}{For further clues, examine the file "./hw1.markdown.err"}') end