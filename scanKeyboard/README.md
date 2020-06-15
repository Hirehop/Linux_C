## Functions:
```
1.int setDisplayMode(int fd,int option)
    rtv:
        success-> 0
        fail   ->-1
    option:
        close scanf input display:0
        open  scanf input display:1

2.int scanKeyboard()
    rtv:
        int data from screen [NOT ASCII CODE]
            W:119
            S:115
            A:97
            D:100

            Enter:10
            ESC:27
            Delete:126
```
