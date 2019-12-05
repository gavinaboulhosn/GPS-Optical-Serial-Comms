from tkinter import *
from tkinter.ttk import Progressbar
from PIL import Image, ImageTk
import socket
import os
import io
import subprocess

CWD=os.getcwd()
# command = ["sudo", "mode2", "-d", "/dev/lirc0"]
command = ["sudo", "ping", "google.com"]
global proc

window = Tk()
window.title("Wireless Communications")
window.geometry('850x250')

#Labels
lbl = Label(window, text = "Expected GPS Packet")
lbl.config(font=("Courier",20))
lbl.grid(column=0,row=1, padx=2, pady=2)

lbl = Label(window, text = "Actual GPS Packet")
lbl.config(font=("Courier",20))
lbl.grid(column=0,row=2, padx=2, pady=2)

lbl = Label(window, text = "Expected vs Actual (Error)")
lbl.config(font=("Courier",20))
lbl.grid(column=0,row=3, padx=2, pady=2)

#Images
imgpy = Image.open(os.path.join(CWD, "Resources/NAVAIR.png"))
img = ImageTk.PhotoImage(imgpy)
Label(window, image = img).grid(row=0, column=3, columnspan=1, rowspan=1, padx=10, pady=2)

lbl = Label(window, text = "Wireless Communications")
lbl.config(font=("Courier", 40))
lbl.grid(column=0, row=0, columnspan=3, rowspan=1, padx=10, pady=2)

GPS_sent = StringVar(window)
gps_sent_label = Label(window, textvariable = GPS_sent)
gps_sent_label.config(font=("Courier",20))
gps_sent_label.grid(column=1, row=1, padx=2, pady=2)

GPS_receive = StringVar(window)
gps_receive_label = Label(window, textvariable = GPS_receive)
gps_receive_label.config(font=("Courier",20))
gps_receive_label.grid(column=1, row=2, padx=2, pady=2)

GPS_error = StringVar(window)
bar = Progressbar(window, length=300, mode='determinate')
bar['value'] = int(GPS_error.get()) if GPS_error.get() is not '' else 0
bar.grid(column=1,row=3, padx=2, pady=2)

error_label = Label(window, text= str(GPS_error.get())+"%")
error_label.config(font=("Courier",20))
error_label.grid(column=2,row=3)

lbl = Label(window, textvariable= GPS_error)
lbl.config(font=("Courier",20))
lbl.grid(column=2,row=3)


def update_gui():
    try:
        for line in io.TextIOWrapper(proc.stdout, encoding="utf-8"):
            message = line.split()
            if (GPS_sent.get() is "0"):
                GPS_sent.set("1")
            else:
                GPS_sent.set("0")
            GPS_receive.set(message[1])
            GPS_error.set(message[0])
            
            try:
                bar['value'] = int(GPS_error.get())
            except ValueError:
                bar['value'] = 0
            
            window.update()
    except KeyboardInterrupt:
        proc.kill()
    


if __name__ == "__main__":
    try:
        proc = subprocess.Popen(command, stdout=subprocess.PIPE)
        GPS_sent.set("0")
        window.after(200, update_gui)
        window.mainloop()
    except KeyboardInterrupt:
        proc.kill()


