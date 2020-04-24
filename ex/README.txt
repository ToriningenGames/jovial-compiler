Document:          <file name of document>
Description:       <Text that describes what's significant about the file;
                       purpose, content, an issue number... why ever it is here.
                       The description can be really long, but for others' sake,
                       keep it to a maximum of 80 columns. For reference, this
                       text is exactly that maximum length.
Expected outcome:  ;

Document:          BLANK.JOV
Description:       An empty file
Expected outcome:  Parse error on line 1
Current outcome:   "Parse error on line 1: Expected START, got end of file"

Document:          PLUS.JOV
Description:       A file containing only a plus sign ('+')
Expected outcome:  Parse error on line 1
Current outcome:   "Parse error on line 1: Expected START, got '+'"

Document:          START.JOV
Description:       A file containing only the word 'START'
Expected outcome:  Parse error on line 1
Current outcome:   "Parse error on line 1: Expected TERM, got end of file"

Document:          EMPTY.JOV
Description:       A file containing a program of only the empty statement
Expected outcome:  No error
Current outcome:   "Parse error on line 2: Expected TERM, got ';'"
