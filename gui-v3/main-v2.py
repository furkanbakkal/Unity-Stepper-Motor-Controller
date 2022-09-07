from decimal import DivisionByZero
import tkinter as tk
import tkinter.ttk as ttk
import serial.tools.list_ports
import time
import threading

usb_connection_status=None
acc_speed_value=50
acc_time_value=3
speed_data=100
pb_value=0
threshold_=0

driver_mode_manuel=True

def send_data(data):
        data=data+"!"
        data=data.encode()
        print(data)
        ser.write(data)

def read_data():
    global speed_data
    global acc_speed_value

    global text2_data
    global text3_data

    global driver_mode_manuel
    global pb
    global pb_value
    global threshold_

    while 1:
        try:

            inp_data=ser.readline()

            if(inp_data!=b''):
                print (inp_data.decode())
                pr_data=[*inp_data.decode()]
                print(pr_data)

                if (pr_data[0]=="h"): #speed
                    speed_data=""
                    for i in range(1,len(pr_data),1):
                        speed_data=str(speed_data)+str(pr_data[i])
                    speed_data=int(speed_data)*10
                    print("speed:",speed_data)
                    text2_data.configure(text="%"+str(speed_data))

                if (pr_data[0]=="a"): #accel
                    acc_speed_value=""
                    for i in range(1,len(pr_data),1):
                        acc_speed_value=str(acc_speed_value)+str(pr_data[i])
                    acc_speed_value=int(acc_speed_value)*10
                    print("acc:",acc_speed_value)
                    text3_data.configure(text="%"+str(acc_speed_value))

                if (pr_data[0]=="m"): #mode
                    if (pr_data[1]=="0"):
                        driver_mode_manuel=True
                        text1_data.configure(text="Manuel")

                    if (pr_data[1]=="1"):
                        driver_mode_manuel=False
                        text1_data.configure(text="Auto")

                if (pr_data[0]=="t"): #threshold
                    threshold_=""
                    for i in range(1,len(pr_data),1):
                        threshold_=str(threshold_)+str(pr_data[i])

                    print("th:",threshold_)
                    threshold_=int(threshold_)*20


                if (pr_data[0]=="q"): #step
                    pb_value=""
                    for i in range(1,len(pr_data),1):
                        if(pr_data[i]=="q"):
                            break

                        if(pr_data[i]=="x"):
                            pb_value=100
                            pb['value'] = pb_value

                        if(pr_data[i]=="y"):
                            pb_value=0
                            pb['value'] = pb_value
                            
                        pb_value=str(pb_value)+ str(pr_data[i])

                    pb_value=(int(pb_value)*10/int(threshold_))*100
                    print("bar2:",pb_value)
                    pb['value'] = int(pb_value)
                    
                if(pr_data[0]=="x"):
                    pb_value=100
                    pb['value'] = pb_value

                if(pr_data[0]=="y"):
                    pb_value=0
                    pb['value'] = pb_value

        except UnicodeDecodeError:
            continue   

        except tk.TclError:
            continue
        
        except ValueError:
            continue
        
        #except ZeroDivisionError:
           # continue

def change_page():
    global root

    for widget in root.winfo_children():
        widget.destroy()
    main_page()

def serial_ports():    
        return serial.tools.list_ports.comports()

def usb_connection(event=None):
        global usb_connection_status
        global comport
        global baud
        global cb1
        global cb2

        if not usb_connection_status:
            comport=cb1.get()
            comport=(comport.split("-"))[0]

            baud=cb2.get()

        print("comboboxes1:",comport )
        print("comboboxes2:", baud)

        global ser
        global text_usb_fail
        global text_usb_ok

        try:
            ser = serial.Serial(port=comport,baudrate = baud, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS, timeout=0.02)
            print("Connected!")

            if  usb_connection_status==False:
                text_usb_fail.destroy()
                
            usb_connection_status=True

            text_usb_ok=tk.Label(root, text="Connected!  "+"(Port: "+comport+", Baudrate: "+baud+")",bg="lightgreen")
            text_usb_ok.pack()
            text_usb_ok.config(font=("Arial", 17))
            text_usb_ok.place(x=160,y=130)

            acc_speed_slider["state"]="normal"

            send_data("d")

            s_read = threading.Thread(target=read_data)
            s_read.start()

        except ValueError or serial.serialutil.SerialException:
        
            if  usb_connection_status:
                text_usb_ok.destroy()
                
            usb_connection_status=False

            text_usb_fail=tk.Label(root, text="Connection Failed !",bg="#FF0063")
            text_usb_fail.pack()
            text_usb_fail.config(font=("Arial", 17))
            text_usb_fail.place(x=300,y=130) 


            print("Error!")
            
        root.mainloop()

def acc_speed_slider_changed(event):
        global acc_speed_value
        global acc_speed_var
        global acc_speed_value_text

        acc_speed_value=(acc_speed_var.get())
        print(acc_speed_value)
        acc_speed_value_text.configure(text="%"+acc_speed_value)

        if(usb_connection_status):
            if(int(acc_speed_value)==100):
                send_data("a"+str(int(int(acc_speed_value)/10)))
                time.sleep(0.1)

            if(int(acc_speed_value)<=90):
                send_data("a0"+str(int(int(acc_speed_value)/10)))
                time.sleep(0.1)

        return acc_speed_value

def center_func():
    print("center func")
    send_data("c")

def mod_change():
    global driver_mode_manuel
    global text1_data
    driver_mode_manuel= not driver_mode_manuel

    if(driver_mode_manuel):
        text1_data.configure(text="Manuel")
        send_data("m0")

    if( not driver_mode_manuel):
        text1_data.configure(text="Auto")
        send_data("m1")

    print(driver_mode_manuel)    

def gogo():
    print("gogo")
    send_data("g")

def go_right():
    global driver_mode_manuel

    if driver_mode_manuel:
        send_data("r0")

    if not driver_mode_manuel:
        send_data("r1")

    print("go right")

def go_left():

    global driver_mode_manuel

    if driver_mode_manuel:
        send_data("l0")

    if not driver_mode_manuel:
        send_data("l1")

    print("go left")

def speed_up():
    global speed_data

    print("speed up")
    speed_data=speed_data+10

    if speed_data>100:
        speed_data=100

    text2_data.configure(text="%"+str(speed_data))

    if(speed_data==100):
        send_data("h"+str(int(int(speed_data)/10)))

    if(speed_data<=90):
        send_data("h0"+str(int(int(speed_data)/10)))

def speed_down():
    global speed_data
    print("speed down")
    speed_data=speed_data-10

    if speed_data<10:
        speed_data=10

    text2_data.configure(text="%"+str(speed_data))

    if(speed_data==100):
        send_data("h"+str(int(int(speed_data)/10)))

    if(speed_data<=90):
        send_data("h0"+str(int(int(speed_data)/10)))

    text2_data.configure(text="%"+str(speed_data))

root = tk.Tk()
root.geometry('800x480')
root.title('Stepper Motor Control')
root.resizable(width=False, height=False)
root["bg"] = "lightgrey"

################## HEADER ################

def main_page():
    global text1_data
    global text2_data
    global text3_data
    global pb

    text1=tk.Label(root, text="Status:")
    text1.pack()
    text1.config(font=("Arial", 20))
    text1.place(x=15,y=15)

    if(driver_mode_manuel):
        text1_data=tk.Label(root, text="Manuel",bg="lightblue")
    else:
        text1_data=tk.Label(root, text="Auto",bg="lightblue")

    text1_data.pack()
    text1_data.config(font=("Arial", 20))
    text1_data.place(x=130,y=15)

    text2=tk.Label(root, text="Speed:")
    text2.pack()
    text2.config(font=("Arial", 20))
    text2.place(x=300,y=15)

    text2_data=tk.Label(root, text="%"+str(speed_data),bg="lightblue")
    text2_data.pack()
    text2_data.config(font=("Arial", 20))
    text2_data.place(x=415,y=15)

    text3=tk.Label(root, text="Acc:")
    text3.pack()
    text3.config(font=("Arial", 20))
    text3.place(x=625,y=15)

    text3_data=tk.Label(root, text="%"+str(acc_speed_value),bg="lightblue")
    text3_data.pack()
    text3_data.config(font=("Arial", 20))
    text3_data.place(x=700,y=15)


    ##################### ARROWS #######################

    arrow_up_photo = tk.PhotoImage(file = "up.png")
    arrow_up_photo = arrow_up_photo.subsample(6, 6)
    arrow_up=tk.Button(root, text="Button-1",height= 125, width=100,image=arrow_up_photo,borderwidth=2,bg="lightgrey",activebackground='grey',command=speed_up)
    arrow_up.pack()
    arrow_up.place(x=60,y=90)

    arrow_down_photo = tk.PhotoImage(file = "down.png")
    arrow_down_photo = arrow_down_photo.subsample(6, 6)
    arrow_down=tk.Button(root, text="Button-1",height= 125, width=100,image=arrow_down_photo,borderwidth=2,bg="lightgrey",activebackground='grey',command=speed_down)
    arrow_down.pack()
    arrow_down.place(x=60,y=240)

    ################

    arrow_right_photo = tk.PhotoImage(file = "r-arrow.png")
    arrow_right_photo = arrow_right_photo.subsample(6, 6)
    arrow_right=tk.Button(root, text="Button-1",height= 100, width=100,image=arrow_right_photo,borderwidth=2,bg="lightgrey",activebackground='grey',command=go_right)
    arrow_right.pack()
    arrow_right.place(x=640,y=110)

    arrow_left_photo = tk.PhotoImage(file = "l-arrow.png")
    arrow_left_photo = arrow_left_photo.subsample(6, 6)
    arrow_left=tk.Button(root, text="Button-1",height= 100, width=100,image=arrow_left_photo,borderwidth=2,bg="lightgrey",activebackground='grey',command=go_left)
    arrow_left.pack()
    arrow_left.place(x=640,y=260)

    ########################## BOTTOM BUTTONS ###################

    bottom_button1_photo = tk.PhotoImage(file = "settings.png")
    bottom_button1_photo = bottom_button1_photo.subsample(9, 9)
    bottom_button1=tk.Button(root, text="Button-1",height= 60, width=150,image=bottom_button1_photo,borderwidth=2,bg="lightgreen",activebackground='grey',command=settings_screen)
    bottom_button1.pack()
    bottom_button1.place(x=40,y=400)

    bottom_button2_photo = tk.PhotoImage(file = "center-align.png")
    bottom_button2_photo = bottom_button2_photo.subsample(6, 6)
    bottom_button2=tk.Button(root, text="Button-1",height= 60, width=150,image=bottom_button2_photo,borderwidth=2,bg="#BC8CF2",activebackground='grey',command=center_func)
    bottom_button2.pack()
    bottom_button2.place(x=230,y=400)

    bottom_button3_photo = tk.PhotoImage(file = "process-arrows.png")
    bottom_button3_photo = bottom_button3_photo.subsample(7, 7)
    bottom_button3=tk.Button(root, text="Button-1",height= 60, width=150,image=bottom_button3_photo,borderwidth=2,bg="orange",activebackground='grey',command=mod_change)
    bottom_button3.pack()
    bottom_button3.place(x=420,y=400)

    bottom_button4_photo = tk.PhotoImage(file = "share.png")
    bottom_button4_photo = bottom_button4_photo.subsample(8, 8)
    bottom_button4=tk.Button(root, text="Button-1",height= 60, width=150,image=bottom_button4_photo,borderwidth=2,bg="lightblue",activebackground='grey',command=gogo)
    bottom_button4.pack()
    bottom_button4.place(x=610,y=400)

    ###############CANVAS##############

    canvas1 = tk.Canvas(height=5,width=770,bg="white",borderwidth=0) #ust
    canvas1.pack()
    canvas1.place(x=10,y=70)

    canvas2= tk.Canvas(height=315,width=5,bg="white",borderwidth=0) #en sag
    canvas2.pack()
    canvas2.place(x=10,y=70)

    canvas3 = tk.Canvas(height=5,width=770,bg="white",borderwidth=0)  #alt
    canvas3.pack()
    canvas3.place(x=10,y=380)

    canvas4 = tk.Canvas(height=315,width=5,bg="white",borderwidth=0) #orta 2
    canvas4.pack()
    canvas4.place(x=590,y=70)

    canvas5 = tk.Canvas(height=315,width=5,bg="white",borderwidth=0) #orta 1
    canvas5.pack()
    canvas5.place(x=200,y=70)

    canvas6 = tk.Canvas(height=315,width=5,bg="white",borderwidth=0)
    canvas6.pack()
    canvas6.place(x=780,y=70)

    """
    canvas7 = tk.Canvas(height=100,width=250,borderwidth=0)
    canvas7.pack()
    canvas7.place(x=275,y=270)
    img = tk.PhotoImage(file="unity.png")      
    canvas7.create_image(125,50 ,image=img)  
    """
    #######################################

    pb_text=tk.Label(root, text="Actuator Position")
    pb_text.pack()
    pb_text.config(font=("Arial", 20))
    pb_text.place(x=305,y=90)

    pb = ttk.Progressbar(root,orient='horizontal',mode='determinate',length=280)
    pb['value'] = 55
    pb.pack()
    pb.place(x=270,y=140)

    ############################

    if usb_connection_status==None or not usb_connection_status:
        info=tk.Label(root, text="Disconnected!",bg="red",fg="white",width=27)
        info.pack()
        info.config(font=("Arial", 18))
        info.place(x=208,y=350)

        arrow_up["state"]="disabled"
        arrow_down["state"]="disabled"
        arrow_right["state"]="disabled"
        arrow_left["state"]="disabled"

        bottom_button2["state"]="disabled"
        bottom_button3["state"]="disabled"
        bottom_button4["state"]="disabled"

    if usb_connection_status:
        info=tk.Label(root, text="Connected!",bg="green",fg="white",width=27)
        info.pack()
        info.config(font=("Arial", 18))
        info.place(x=208,y=350)

        arrow_up["state"]="normal"
        arrow_down["state"]="normal"
        arrow_right["state"]="normal"
        arrow_left["state"]="normal"

        bottom_button2["state"]="normal"
        bottom_button3["state"]="normal"
        bottom_button4["state"]="normal"

    root.mainloop()

def settings_screen():
    global root
    global cb1
    global cb2

    global acc_speed_value
    global acc_speed_value_text
    global acc_speed_var

    global acc_speed_slider

    for widget in root.winfo_children():
       widget.destroy()

    baudrate_choices=["9600","115200","19200"]
    back_button_photo = tk.PhotoImage(file = "undo.png")
    back_button_photo = back_button_photo.subsample(8,8)
    back_button=tk.Button(root, text="Button-1",height= 70, width=70,image=back_button_photo,borderwidth=2,bg="#6FB2D2",activebackground='grey',command=change_page)
    back_button.pack()
    back_button.place(x=355,y=380)

    text2_1=tk.Label(root, text="COM Port")
    text2_1.pack()
    text2_1.config(font=("Arial", 20))
    text2_1.place(x=125,y=15)

    text2_2=tk.Label(root, text="Baudrate")
    text2_2.pack()
    text2_2.config(font=("Arial", 20))
    text2_2.place(x=350,y=15)

    text2_3=tk.Label(root, text="Connect")
    text2_3.pack()
    text2_3.config(font=("Arial", 20))
    text2_3.place(x=565,y=15)

    connect_button_photo = tk.PhotoImage(file = "usb.png")
    connect_button_photo = connect_button_photo.subsample(10,10)
    connect_button=tk.Button(root, text="Button-1",height= 35, width=150,image=connect_button_photo,borderwidth=2,bg="#FFEE63",activebackground='grey',command=usb_connection)
    connect_button.pack()
    connect_button.place(x=540,y=69)

    cb1 = ttk.Combobox(root, values=serial_ports(),font=("Ariel", 20),width=10)
    cb1.pack()
    cb1.place(x=100,y=70)
    #cb1.bind('<<ComboboxSelected>>', on_select)

    cb2 = ttk.Combobox(root, values=baudrate_choices,font=("Ariel", 20),width=10)
    cb2.pack()
    cb2.place(x=320,y=70)
    #cb2.bind('<<ComboboxSelected>>', on_select)

    canvas2_1 = tk.Canvas(height=5,width=770,bg="white",borderwidth=0) #ust
    canvas2_1.pack()
    canvas2_1.place(x=10,y=175)

    ###################  ACC ################

    text2_3=tk.Label(root, text="Acceleration: ")
    text2_3.pack()
    text2_3.config(font=("Arial", 20))
    text2_3.place(x=260,y=220)

    acc_speed_var=tk.StringVar()
    acc_speed_slider = tk.Scale(root, from_=10, to=100,length=300, resolution=10,background="#9772FB", foreground="white",orient="horizontal",width=30,variable=acc_speed_var,command=acc_speed_slider_changed,activebackground="grey")
    acc_speed_slider.set(acc_speed_value)
    acc_speed_slider.pack()
    acc_speed_slider.place(x=240,y=270)
    acc_speed_slider.configure(repeatdelay=6000)

    acc_speed_value_text=tk.Label(root, text=acc_speed_value,bg="lightblue")
    acc_speed_value_text.pack()
    acc_speed_value_text.config(font=("Arial", 20))
    acc_speed_value_text.place(x=450,y=220)


    ###########

    if  usb_connection_status:
        text_usb_ok=tk.Label(root, text="Connected!  "+"(Port: "+comport+", Baudrate: "+baud+")",bg="lightgreen")
        text_usb_ok.pack()
        text_usb_ok.config(font=("Arial", 17))
        text_usb_ok.place(x=160,y=130)

        
        
    if  not usb_connection_status or usb_connection_status==None :
        acc_speed_slider["state"]="disabled"

    root.mainloop()

main_page()

