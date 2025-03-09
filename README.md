# ARCHWAY Project Manager

<img align="center" width="800" src="https://github.com/user-attachments/assets/e9861b4d-b5a2-4717-8761-48509e547862">

### *SETUP*:
*No setup is required;* just download the project and load the **.pro** file from within Qt Creator to modify the code


### **GETTING THE APP:**
- Step 1: dowload the *dist.zip* in your local pc
- Step 2: Unzip the folder and rename it as ArchWay
- Step 3: Place the folder in any directory you would like, beside the ones that required AdminPrivilages for saving
  because the Groups are stored in a json document and saved to it every time you close the app.
  
  ```sh
  Missing to follow the above directive, will cause a "Permission Denied" error by the system
  complaining about the application not having access to modify the json file
  ```
- Step 4: [optional] You can open the project dir and right-clicking on the *ArchWay.exe* you can add it to the taskbar for easy access. 



### KEEP IN TOUCH: 🤜🤛 👨‍💻 
> for report bugs, suggest features and improvements or ask questions about the program's implementation details,
> send me an email at **aventurelli.github@gmail.com**



### **DESCRIPTION:**
🚀 the scope of this app is to manage various projects with ease and few clicks! 
How annoying is to open the same folders and files every day before start working? and what about if you dont remember the location? it's frustrating, doesn't it? 💢

For these resons Archway is the solution.. 
Archway is a *100+ hours* project which has the capability to group any number of files and directory and giving it a name, so the next time you only need one **CLICK** to open all the files/dir at ones! ⚡


### **key futures:**
1. Change path from the Table by double-clicking the path and the program will prompt to choose a new path (file or dir)
2. toggle switch "Openable". Every file that has the toggle off, will not be opend when the Group's button "Open All" is clicked.
3. Under the table, you find a card that display the current selected row on the table, to better see the information.


### APP DETAILS
This app implement many custom widgets, models and delegates. Some of them are:
1. Toggle Switch -> based on QCheckBox
2. Toggle chip -> has two text, with the selected option framed inside a rounded rectangle
3. PathTypeSelectorDialog -> opens a dialog that let the user choose whether to select a dir or file. This is necessary because Qt doesn't allow direct selection of both files and dir.  


### Future improvement list:
- add a search bar for filtering the Groups.
- automatic re-naming within the editor (aimed for that file who's name will be changed dayly.. es: myTextFile 19Jan2025.txt)
