Document:           <file name of document>
Description:        <Text that describes what's significant about the file;
                        purpose, content, an issue number... why ever it is here.
                        The description can be really long, but for others' sake,
                        keep it to a maximum of 80 columns. For reference, this
                        text is exactly that maximum length.
Expected outcome:   If an error is expected, what kind of error, and where? Why?
                        If no error, anything specific the resultant program
                        should do?
Current outcome:    What does the compiler actually do? What does the produced
                        program actually do?
Pass/Fail:          It should obviously show between Expected and Current if the
                        test passes or fails. This field is for the computer's
                        convenience, in case of future automated tests.

Document:           BLANK.JOV
Description:        An empty file
Expected outcome:   Parse error on line 1
Current outcome:    "Parse error on line 1: Expected START, got end of file"
Pass/Fail:          Pass

Document:           PLUS.JOV
Description:        A file containing only a plus sign ('+')
Expected outcome:   Parse error on line 1
Current outcome:    "Parse error on line 1: Expected START, got '+'"
Pass/Fail:          Pass

Document:           START.JOV
Description:        A file containing only the word 'START'
Expected outcome:   Parse error on line 1
Current outcome:    "Parse error on line 1: Expected TERM, got end of file"
Pass/Fail:          Pass

Document:           EMPTY.JOV
Description:        A file containing a program of only the empty statement
Expected outcome:   No error
Current outcome:    No error
Pass/Fail:          Pass

