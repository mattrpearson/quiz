# quiz


Q: Given a list of words like https://github.com/NodePrime/quiz/blob/master/word.list find the longest compound-word in the list, which is also a concatenation of other sub-words that exist in the list. The program should allow the user to input different data. The finished solution shouldn't take more than one hour. Any programming language can be used, but Go is preferred.


Fork this repo, add your solution and documentation on how to compile and run your solution, and then issue a Pull Request. 

Obviously, we are looking for a fresh solution, not based on others' code.


How to execute my solution:

make
./longest_compound word.list


The solution itself is pretty simple.  The whole list of words is read in
from the file and each word is inserted into two structures, a trie and a
list.  The list is then iterated over and the trie is used to check for
matching substrings.  When a word match is made, the remaining portion of the
word will be recursively checked against the root of the trie.
