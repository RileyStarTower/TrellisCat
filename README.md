# TrellCat
Writing tool written in Qt (C++) that lets you focus on the structure of a narrative rather than the prose.

The app doesn't support new file creation yet. Creating a file by scratch isn't too difficult though. The app accepts .tcpro files, or .🐱 files, although Linux doesn't currently support .🐱 files completely.
The contents of the file are a pre-order traversal or a tree, where each node of the tree takes up a single line, with the following format:

`<NODE DEPTH> | <NODE CONTENTS>`

The contents portion is optional, everything else is required. For example:

```
1|Beginning
1|Middle
2|Chapter 1
3|Scene 1
2|Chapter 2
3|Scene 1
3|Scene 2
1|End
```


Would result in the following tree:

```
Beginning
Middle  ----  Chapter 1   ----  Scene 1
        ----  Chapter 2   ----  Scene 1
                          ----  Scene 2
End
```

Note that the tree starts at depth 1. This is because having a single node at the left of the screen doesn't sound like a useful feature, but the underlying code still creates a node at depth 0 that is the parent to all the depth-1 nodes.
