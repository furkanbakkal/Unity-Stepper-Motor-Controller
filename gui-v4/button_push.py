from tkinter import * 
running = False
root = Tk()
jobid=None

def start_motor(event):
    global running
    global jobid
    running = True
    print("starting motor...")
    jobid=root.after(100, start_motor,event)

def stop_motor(event):
    global running
    global jobid
    print("stopping motor...")
    root.after_cancel(jobid)
    running = False

button = Button(root, text ="forward")
button.pack(side=LEFT)
button.bind('<ButtonPress-1>',start_motor)
button.bind('<ButtonRelease-1>',stop_motor)
root.mainloop()