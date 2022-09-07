import tkinter as tk
import tkinter.ttk as ttk
import serial.tools.list_ports

# --- functions ---

def serial_ports():    
    return serial.tools.list_ports.comports()

def on_select(event=None):

    # get selection from event    
    print("event.widget:", event.widget.get())

    # or get selection directly from combobox
    print("comboboxes: ", cb.get())

def connect_usb():
    return

baudrate_choices=["9600","a","b"]


root = tk.Tk()
root.geometry('800x550')
root.resizable(width=False, height=False)
root["bg"] = "lightgrey"

cb = ttk.Combobox(root, values=serial_ports())
cb.pack()
cb.place(x=10,y=25)
cb.bind('<<ComboboxSelected>>', on_select)

cb = ttk.Combobox(root, values=baudrate_choices)
cb.pack()
cb.place(x=175,y=25)
cb.bind('<<ComboboxSelected>>', on_select)

header_text1 = tk.Label(root, text="Select COM ")
header_text1.place(x = 50,y = 2)  

header_text2 = tk.Label(root, text="Select Baudrate ")
header_text2.place(x = 205,y = 2)  

header_hr = tk.Label(root, text="----------"*100)
header_hr.place(x = 0,y = 50) 
header_hr["bg"]="white" 

connect_btn = tk.Button(root, text ="Connect", command = connect_usb,width=25)
connect_btn.pack()
connect_btn.place(x=340,y=20)

##############################################################################

photo_btn1 = tk.PhotoImage(file = "")
photo_btn1 = photo_btn1.subsample(3, 3)
auto_top=tk.Button(root, text="Button-1",height= 125, width=55,image=photo_btn1)
auto_top.pack()
auto_top.place(x=160,y=100)

photo_btn2 = tk.PhotoImage(file = "")
photo_btn2 = photo_btn2.subsample(3, 3)
auto_bottom=tk.Button(root, text="Button-2",height= 125, width=55,image=photo_btn2)
auto_bottom.pack()
auto_bottom.place(x=160,y=300)

photo_btn3 = tk.PhotoImage(file = "")
photo_btn3 = photo_btn3.subsample(3, 3)
manuel_right=tk.Button(root, text="Button-3",height= 55, width=125,image=photo_btn3)
manuel_right.pack()
manuel_right.place(x=238,y=238)

photo_btn4 = tk.PhotoImage(file = "")
photo_btn4 = photo_btn4.subsample(3, 3)
manuel_left=tk.Button(root, text="Button-4",height= 55, width=125,image=photo_btn4)
manuel_left.pack()
manuel_left.place(x=10,y=240)

photo_home = tk.PhotoImage(file = "")
photo_home = photo_home.subsample(3, 3)
center_mid=tk.Button(root, text="Center",height= 50, width=50,image=photo_home)
center_mid.pack()
center_mid.place(x=162,y=238)

header_hr = tk.Label(root, text="----------"*100)
header_hr.place(x = 0,y = 450)  
#############################################################################

speed_slider = tk.Scale(root, from_=100, to=10,length=300, resolution=10,background="red", foreground="white")
speed_slider.set(50)
speed_slider.pack()
speed_slider.place(x=450,y=110)


##############################################################################
custom_command = tk.StringVar()

text_cc = ttk.Label(root, text="Command:")
text_cc .pack(fill='x', expand=True)
text_cc.place(x=10,y=484)


cc_entry = ttk.Entry(root, textvariable=custom_command,width=110)
cc_entry.pack(fill='x', expand=True)
cc_entry.place(x=75,y=484)
cc_entry.focus()

##############################################################################

text_info = ttk.Label(root, text="Info:",width=10)
text_info.pack()
text_info.place(x=10,y=510)

out_info = tk.Text(root, height = 1, width = 83,bg = "#D3CEDF")
out_info.pack()
out_info.place(x=75,y=510)


root.mainloop()