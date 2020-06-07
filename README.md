## About FA2Copy - RN Specific Version
FA2Copy is an extension DLL which provide some fixes for FinalAlert 2. With it, you can use FA2 more conveniently.

FA2Copy uses message hook and Windows API to achieve its functions, and Zero Fanker introduced inline hook method from YR-Ares syringe way .

## How to use
1. Prepare the dll. Simply, you can pull this project and compile it with X86. I had compiled one, you can just find it in the **examples** folder.
2. Not ready for the public to use yet, currently designed for YRMOD - Revenge Now only .
3. Work along with **FA2Ext.dll** properly, which is developed by AlexB.
5. You should configure 'FA2CopyData.ini' correctly, as FA2Copy uses functions like FindWindow and will get window's handle by its name, it's really necessary. Also I will provide you a sample ini file just in the **examples** folder, if something works wrong while using FA2Copy, always remember to check your configure file before reporting the bug.

## More information
I put a text file(controls.txt) in **samples** as well, you can find all controls need modifying from it.

It's a little bit complex for me to add an accelerate without MFC, now I'm working on it. But for know, FA2Copy will only use a GlobalAtom to realize the hotkey. It means you cannot use Ctrl+S/O/N with FinalAlert2 open for now. You can help to me fix or improve FA2Copy if you like and have time to do it. I'll be very happy for your help. (Maybe we can just focus on working on a map instead of editing something at the same time?)

Also, in the future I will consider to use an injector instead of modify it by hand. However, as it's not necessary as I have to modify the program anyway to add and adjust the controls, I'm not sure this will be really realized in the future.

My code style maybe not standard and have some mistake through the code, so I'd be happy to get your suggestions if it's good to improve my skills. In fact, it's my first project, after no longer working on the algorithm competition.

## Links
[Stuffhost, you can get FA2Ext from here.](http://www.stuffhost.de/ares/)

[A version migrated to FinalAlert2 by Caco, download password is 'caco'.](https://pan.baidu.com/s/1KiJFochCFo9JJpCBYCRDIg)

[The version for Mental Omega use can be easily found in my personal blog.](https://secsome.github.io/post/00002/)

[My bilibili space.](https://space.bilibili.com/143748001)
