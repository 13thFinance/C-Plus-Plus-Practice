program is working mostly fine except for one bug:  
Bug: On CTRL - V after loading the shifted string into the clipboard, it prints out garbage. On further inspection, manually pasting shows that the clipboard contains the right data, It just prints out something weird for whatever reason. Double check the paste     function.
may be that it needs a small delay to load data properly.
