#!/usr/bin/python

from PIL import Image, ImageDraw, ImageFont
import paho.mqtt.client as mqtt
import datetime


image_size = (300,90)
max_watts = 400.
fn = "/tmp/trainer_stats.png"
fnt = ImageFont.truetype('/usr/share/cups/fonts/FreeMono.ttf', 40)
time_horizon = 20*60*1000 #20 mins
time_resolution = time_horizon/image_size[0] #ms

#print "time_resolution=",time_resolution

diagram_plan = []
diagram_cur  = []

def redner(fn, current, aim, diagrams=[]):
	global max_watts, fnt, image_size
	
	for diagram in diagrams:
		max_watts = max(max_watts, max([w[1] for w in diagram[1]]))
	
	im = Image.new('RGBA', image_size, (255,255,255,0))
	
	# get a drawing context
	d = ImageDraw.Draw(im)

	# draw text, half opacity
	d.text((10,30), str(current), font=fnt, fill=(0,0,0,255))
	# draw text, full opacity
	dif = min(1.,max(0.,abs(current-aim)-20)/20.)
	print dif
	
	d.text((150,30), str(aim), font=fnt, fill=(int(round(255*dif)),0,0,255))
	
	for diagram in diagrams:
		last = None
		for v in diagram[1]:
			cur = (im.size[0]*v[0], im.size[1]*(1-v[1]/max_watts))
			if last!=None:
				d.line((last[0], last[1], cur[0], cur[1]), fill=diagram[0] )
			last = cur
		if last!=None and last[1]!=im.size[1]:
			d.line((last[0], im.size[1], last[0], last[1]), fill=diagram[0] )
	
	#im.show()
	im.save(fn, "PNG")
	
def redner_msg(fn, msg):
	global fnt, image_size
	
	im = Image.new('RGBA', image_size, (255,255,255,0))
	
	# get a drawing context
	d = ImageDraw.Draw(im)

	# draw text, half opacity
	d.text((10,30), str(msg), font=fnt, fill=(0,0,0,255))
	
	im.save(fn, "PNG")
	

def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))

    client.subscribe("trainer_stats/#")
    
def shift(d, delta):
	r=[]
	for i in xrange(len(d)):
		if d[i][0]>=delta:
			r.append([d[i][0]-delta, d[i][1]])
	return r

last_ts = datetime.datetime.now()
def on_message(client, userdata, msg):
    global fn, diagram_cur, diagram_plan, last_ts, time_resolution, image_size
	
    print(msg.topic + " " + str(msg.payload))
    
    data = msg.payload.split(";")
    values = [float(d) for d in data]
    
    if len(values)==4:
		(cur, aim, aim5, aim10) = values
		
		#elapsed time since last msg
		cur_ts= datetime.datetime.now()
		diff = cur_ts - last_ts
		last_ts = cur_ts
		elapsed_ms = (diff.days * 86400000) + (diff.seconds * 1000) + (diff.microseconds / 1000)
		
		#built diagram
		diagram_plan = shift(diagram_plan, int(round(elapsed_ms/float(image_size[0]*time_resolution))))
		diagram_cur = shift(diagram_cur, int(round(elapsed_ms/float(image_size[0]*time_resolution))))
		
		diagram_plan.append([0.50, aim])
		diagram_plan.append([0.75, aim5])
		diagram_plan.append([1.00, aim10])
		diagram_cur.append([0.5, cur])
		
		diagram_plan.sort()
		
		#pairs of (percent of time, aimed watts)
		diagram1 = ((0,0,0,255), diagram_plan)
		diagram2 = ((255,0,0,255), diagram_cur)

		redner(fn, 22,234, [diagram1,diagram2])

redner_msg(fn, "  Start!")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("localhost", 1883, 60)

client.loop_forever()
