## Functions:
```
1.int setDisplayMode(int fd,int option)
    rtv:
        success-> 0
        fail   ->-1
    option:
        close cursor display:0
        open  cursor display:1

2.int scanKeyboard()
    rtv:
        int data from screen [NOT ASCII CODE]
```
