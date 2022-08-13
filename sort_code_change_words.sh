# version 1.0

cat code_change_words_base | awk '{ print length, $0 }' | sort -n -s -r | cut -d" " -f2- > code_change_words