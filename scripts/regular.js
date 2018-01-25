//print(x);
/*x=x+1;

print(mqtt.isConnected());
mqtt.publish(1,"topic","msg"+x);

print(elapsed);

if(elapsed>1) true;
else false;*/

load = rtData.getLoad();

if(load>50)	//start workout replay if trainer is actually used
	regularEvent.startWorkout();

if(load!=last) {
	print(elapsed+": "+load);
	last = load;
}
