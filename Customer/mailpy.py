import smtplib
import sys
from email.MIMEMultipart import MIMEMultipart
from email.MIMEText import MIMEText
 
#print sys.argv
#print sys.argv[1]

fromaddr = "srivastava.pallak@gmail.com"
toaddr = sys.argv[1]
msg = MIMEMultipart()
msg['From'] = fromaddr
msg['To'] = toaddr
msg['Subject'] = "Booking Confirmation"
 
body = "Your tickets have been booked!!Happy Journey!!."
msg.attach(MIMEText(body, 'plain'))
 
server = smtplib.SMTP('smtp.gmail.com', 587)
server.starttls()
server.login(fromaddr, "1pe09ec063@")
text = msg.as_string()
server.sendmail(fromaddr, toaddr, text)
server.quit()
