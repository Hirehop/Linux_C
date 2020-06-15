Functions:
```
1.int setDisplayMode(int fd,int option)</br>
    rtv:
        success->0  </br>
        fail   ->-1 </br>
    option:
        close cursor display:0
        open  cursor display:1

2.int scanKeyboard()
    rtv:
        int data from screen [NOT ASCII CODE]
```
