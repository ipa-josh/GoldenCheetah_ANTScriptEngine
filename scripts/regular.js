//print(x);
/*x=x+1;

print(mqtt.isConnected());
mqtt.publish(1,"topic","msg"+x);

print(elapsed);

if(elapsed>1) true;
else false;*/

load = rtData.getLoad();

if(rtData.getWatts()>50)	//start workout replay if trainer is actually used
	regularEvent.startWorkout();

//current watt, current aim, aim in 5 min, aim in 10 min
mqtt.publish(1,"/trainer_stats",rtData.getInterpolatedWatts()+";"+rtData.getLoad()+";"+regularEvent.loatAtRelative(1000*60*5)+";"+regularEvent.loatAtRelative(1000*60*10));

print("interpolated Watts: "+rtData.getInterpolatedWatts());

print(regularEvent.loatAtRelative(0)+" "+regularEvent.loatAtRelative(1000*60*5));

if(load!=last) {
	print(elapsed+": "+load);
	last = load;
}
