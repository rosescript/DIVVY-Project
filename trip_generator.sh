#grabbing the json data 
curl https://data.cityofchicago.org/resource/fg6s-gzvg.json

# takes the data we want from the api, then messes around with it to remove
# some ugly formatting, makes it pretty and the outputs it to trip_data.txt
cat trip_data.txt | jq ".[0:71] | .[0:71] | .[] |[.trip_id,.bike_id,.from_station_id,.to_station_id,.trip_duration,.start_time] | @csv" trips.json | tr ',' ' ' | sed 's/"//;s/"//;s/"//;s/"//;s/"//;s/"//;s/"//;s/"//;s/"//;s/"//;s/"//;s/"//;s/"//;s/"//;s/20..-..-..T//;s/\\//;s/\\//;s/\\//;s/\\//;s/\\//;s/\\//;s/\\//;s/\\//;s/\\//;s/\\//;s/\\//;s/\\//;s/.000//' > trip_data.txt

# making sure the output is what we want
cat trip_data.txt