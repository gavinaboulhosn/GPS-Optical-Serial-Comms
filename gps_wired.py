from tkinter import *
from tkinter.ttk import Progressbar
from PIL import Image, ImageTk
import socket
import os

CWD=os.getcwd()

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

GPS_sent = StringVar()
GPS_sent.set("")
gps_sent_label = Label(window, textvariable = GPS_sent)
gps_sent_label.config(font=("Courier",20))
gps_sent_label.grid(column=1, row=1, padx=2, pady=2)

GPS_receive = StringVar()
GPS_receive.set("")
gps_receive_label = Label(window, textvariable = GPS_receive.get())
gps_receive_label.config(font=("Courier",20))
gps_receive_label.grid(column=1, row=2, padx=2, pady=2)

GPS_error = StringVar()
GPS_error.set(0)
bar = Progressbar(window, length=300, mode='determinate')
bar['value'] = GPS_error.get()
bar.grid(column=1,row=3, padx=2, pady=2)

error_label = Label(window, text= str(GPS_error.get())+"%")
error_label.config(font=("Courier",20))
error_label.grid(column=2,row=3)


def update_gui():
    message = client.receive().split(',')
    GPS_sent = message[0]
    GPS_receive = message[1]
    GPS_error = message[2]
    window.after(1000, update_gui)

if __name__ == "__main__":
    # update_gui()
    window.mainloop()


