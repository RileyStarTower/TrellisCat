# TrellCat
Writing tool written in Qt (C++) that lets you focus on the structure of a narrative rather than the prose.

The app doesn't support new file creation yet. Creating a file by scratch isn't too difficult though. The app accepts .tcpro files, or .🐱 files, although Linux doesn't currently support .🐱 files completely.
The contents of the file are a pre-order traversal or a tree, where each node of the tree takes up a single line, with the following format:

\<NODE DEPTH\> | \<NODE CONTENTS\>

The contents portion is optional, everything else is required. For example:

```
1|Prologue
1|Chapter 1
2|Scene 1
2|Scene 2
1|The End
```


Would result in the following tree:

```
Prologue
Chapter 1   ----  Scene 1
            ----  Scene 2
The End
```
