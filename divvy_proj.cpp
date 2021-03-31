//
// Author: Sydney Wells, UIC, Spring 2021
// Assignment: Project 4
//

//
// so in this project i'll be creating a program that 
// will basically take in DIVVY data, and then the user
// can mess around with commands that will output things
// like a trip summary, or nearby DIVVY stations, etc
//

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <cctype>

using namespace std;

// struct for station info
struct Station {
    string stationID;
    int capacity;
    double latitude;
    double longitude;
    string stationName;
};

// struct for bike and trip info
struct BikeTrips {
    string tripID;
    string bikeID;
    string startStation;
    string endStation;
    int tripDuration;
    string startTime;

};

// struct for nearme function
// its basically the same as Station
// but adds distance and gets rid of
// lat, long, and capacity
struct NearbyStation {
    string stationID;
    int capacity;
    string stationName;
    double distance;
};




//
// inputStationData function
// takes the station data file, opens it, copies its 
// contents into an array, and then closes it
// status: working just fine!
//

Station* inputStationData(string filename, int& S) {
    ifstream infile;
    infile.open(filename);

    // make sure the file can be opened
    // if not, returns nullptr
    if (!infile.good()) {
        S = 0;
        return nullptr;
    }
    infile >> S;

    // dynamically-allocating an array
    // for the stations
    Station* station = new Station[S];

    for (int i = 0; i < S; ++i) {
        infile >> station[i].stationID;
        infile >> station[i].capacity;
        infile >> station[i].latitude;
        infile >> station[i].longitude;
        getline(infile, station[i].stationName);
        station[i].stationName.erase(0, 1);
    }
    return station;
}

//
// inputTripData function
// basically the same as the inputStationData
// function, except it handles the BikeTrip struct
// and inputs the appropriate data into an array of
// BikeTrips structs 
//

BikeTrips* inputTripData(string filename, int& T) {
    ifstream infile;
    infile.open(filename);
    
    // make sure the file can be opened
    // if not, returns nullptr
    if (!infile.good()) {
        T = 0;
        return nullptr;
    }
    infile >> T;

    // dynamically-allocating an array
    // for the trips
    BikeTrips* trip = new BikeTrips[T];

    for (int i = 0; i < T; ++i) {
        infile >> trip[i].tripID;
        infile >> trip[i].bikeID;
        infile >> trip[i].startStation;
        infile >> trip[i].endStation;
        infile >> trip[i].tripDuration;
        infile >> trip[i].startTime;
    }
    return trip;
}


//
// quickStats
// this function takes in the arrays for the stations
// and trips, and it counts the # of stations, trips,
// and the total capacity across all stations 
//

void quickStats(Station station[], int& S, int& T) {
    int bc_count = 0;

    // loops through the station list
    // and adds the bike capacities
    // to bc_count
    for (int i = 0; i < S; ++i) {
        bc_count += station[i].capacity;
    }

    // output the stats
    cout << "   stations: " << S << endl;
    cout << "   trips: " << T << endl;
    cout << "   total bike capacity: " << bc_count << endl;
}

//
// distBetween2Points
//
// Returns the distance in miles between 2 points (lat1, long1) and 
// (lat2, long2).  Latitudes are positive above the equator and 
// negative below; longitudes are positive heading east of Greenwich 
// and negative heading west.  Example: Chicago is (41.88, -87.63).
//
// NOTE: you may get slightly different results depending on which 
// (lat, long) pair is passed as the first parameter.
// 
// Originally written by: Prof. Hummel, U. of Illinois, Chicago, Spring 2021
// Reference: http://www8.nau.edu/cvm/latlon_formula.html
//
double distBetween2Points(double lat1, double long1, double lat2, double long2)
{
  double PI = 3.14159265;
  double earth_rad = 3963.1;  // statue miles:

  double lat1_rad = lat1 * PI / 180.0;
  double long1_rad = long1 * PI / 180.0;
  double lat2_rad = lat2 * PI / 180.0;
  double long2_rad = long2 * PI / 180.0;

  double dist = earth_rad * acos(
    (cos(lat1_rad) * cos(long1_rad) * cos(lat2_rad) * cos(long2_rad))
    +
    (cos(lat1_rad) * sin(long1_rad) * cos(lat2_rad) * sin(long2_rad))
    +
    (sin(lat1_rad) * sin(lat2_rad))
  );

  return dist;
}


//
// tripDurations
// takes in the trip array, and loops
// through it a few times in order to
// count the amount of trips that were 
// x amount of minutes or x amount of hours
// and outputs them
//

void tripDurations(BikeTrips trip[], int& T) {
    int zeroThrity = 0;
    int thirtySixty = 0;
    int oneTwo = 0;
    int twoFive = 0;
    int fivePlus = 0;

    // counting trips 30 min or less
    for (int i = 0; i < T; ++i) {
        // counting trips longer than 5 hours
        if (300 < (trip[i].tripDuration / 60)) {
            fivePlus++;
        } // counting trips 2 to 5 hours
         else if ((120 < (trip[i].tripDuration /60)) && ((trip[i].tripDuration/60) <= 300)) {
            twoFive++;
        } // counting trips 1 to 2 hours
         else if ((60 < (trip[i].tripDuration / 60 )) && ((trip[i].tripDuration / 60) <= 120)) {
            oneTwo++;
        } // counting trips 1 to 2 hours
         else if ((30 < (trip[i].tripDuration / 60)) && ((trip[i].tripDuration / 60) <= 60)) {
            thirtySixty++;
        } // counting trips 30-60 mins
         else if ((trip[i].tripDuration / 60) <= 30) {
            zeroThrity++;
        }
    }

    //outputting the counts
    cout << "   trips <= 30 mins: " << zeroThrity << endl;
    cout << "   trips 30..60 mins: " << thirtySixty << endl;
    cout << "   trips 1-2 hrs: " << oneTwo << endl;
    cout << "   trips 2-5 hrs: " << twoFive << endl;
    cout << "   trips > 5 hrs: " << fivePlus << endl;

}

//
// tripCounter
// helper function for listAllStations
// counts the number of trips a 
// station has been the starting 
// or endpoint for
//

int tripCounter(string stationid, BikeTrips trip[], int& T) {

    // counter for amt of trips
    int j = 0;

    // loops through the trip array
    // and checks to see if the stationid
    // matches the start or end station,
    // if there's a match, count increases
    for (int i = 0; i < T; ++i) {
        if (trip[i].startStation == trip[i].endStation) {
            if (trip[i].startStation == stationid) {
                j++;
            }
        } else if (trip[i].startStation != trip[i].endStation) {
            if (trip[i].startStation == stationid) {
                j++;
            } else if (trip[i].endStation == stationid) {
                j++;
            }
        }
    }
    return j;
}

//
// listAllStations
// takes all of the stations and lists them in
// alphabetiical order
//

void listAllStations(Station station[], int& S, BikeTrips trip[], int& T)
{
    // sorting through the station array
    // and ordering it alphabetically
    for (int i = 0; i < S-1; i++)
   {
      int minIndex = i;
      
      for (int j = i+1; j < S; j++)
      {
         if (station[j].stationName < station[minIndex].stationName)
         {
            minIndex = j;
         }
      }
      
      Station temp = station[i];
      station[i] = station[minIndex];
      station[minIndex] = temp;
   }

   // outputting the new array with info
   for (int i = 0; i < S; ++i) {
       cout << station[i].stationName << " (" << station[i].stationID << ") @ (" << station[i].latitude << ", " << station[i].longitude << "), " << station[i].capacity << " capacity, " << tripCounter(station[i].stationID, trip, T) << " trips" << endl;
   }
}

// binary search function
// pulled from zybooks 9.7
int binarySearch(int numbers[], int N, int key) {
   int mid;
   int low;
   int high;
   
   low = 0;
   high = N - 1;
   
   while (high >= low) {
      mid = (high + low) / 2;
      if (numbers[mid] < key) {
         low = mid + 1;
      }
      else if (numbers[mid] > key) {
         high = mid - 1;
      }
      else {
         return mid;
      }
   }
   
   return -1; // not found
}

//
// startTimes
// goes through the trips and 
// lists the number of times
// a trip started within a 
// certain hour
//

void startTimes(BikeTrips trip[], int T) {
    
    // creating an array with all the possible times
    int hrs[24] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
    // // creating a counter array to give each hour a category
    int times[24] = {0};
    int hour = 0;
    int index = 0;
    
    for (int i = 0; i < T; ++i) {
        hour = stoi(trip[i].startTime.substr(0,2));
        index = binarySearch(hrs, 24, hour);
        if (index != -1) {
            times[index] += 1;
        }
    }
    
    for (int i = 0; i < 24; ++i) {
        cout << hrs[i] << ": " << times[i] << endl;
    }
}

// distSort
// sorts stations by distance,
// nearest to farthest
// it's selection sort just
// tweaked to sort distances

void distSort(NearbyStation nb[], int N) {
   for (int i = 0; i < N-1; i++)
   {
      int minIndex = i;
      
      for (int j = i+1; j < N; j++)
      {
         if (fabs(nb[j].distance) < nb[minIndex].distance)
         {
            minIndex = j;
         }
      }
      
      NearbyStation temp = nb[i];
      nb[i] = nb[minIndex];
      nb[minIndex] = temp;
   }
}


// nearbyStations
// searches for stations nearby 
// a given latitude and longitude
// withing D miles and outputs those
// stations and the distance

void nearbyStations(Station station[], int S, double D, double lat1, double long1) {
    int counter = 0;
    double dist = 0.0;

    
    // looping through to see how many
    // stations are within range
    for (int i = 0; i < S; ++i) {
        dist = distBetween2Points(lat1, long1, station[i].latitude, station[i].longitude);
        if (fabs(dist) <= D) {
            counter++;
        }
    }
    // cout << counter << endl;
    if (counter == 0) {
        cout << "The following stations are within " << D << " miles of (" << lat1 << ", " << long1 << "):" << endl;
        cout << "none found" << endl;
    } else {
        //creating a new temp array to store
        // and sort the distances within range
        NearbyStation* temp = new NearbyStation[counter];

        // looping through again to insert
        // the stations into the temp array
        int j = 0;
        for (int i = 0; i < S; ++i) {
            dist = distBetween2Points(lat1, long1, station[i].latitude, station[i].longitude);
            if (fabs(dist) <= D) {
                temp[j].stationID = station[i].stationID;
                temp[j].stationName = station[i].stationName;
                temp[j].distance = dist;
                j++;
            }
        }

        // need to sort the array
        distSort(temp, counter);

        // output the nearby stations!!
        cout << "The following stations are within " << D << " miles of (" << lat1 << ", " << long1 << "):" << endl;
        for (int i = 0; i < counter; ++i) {
            cout << "station " << temp[i].stationID << " (" << temp[i].stationName << "): " << temp[i].distance << " miles" << endl;
        }
        delete[] temp;

    }
    

}

//
// findStations
// given a string input, 
// this function searches for
// any station that contains
// that input, case sensitive
//
void findStations(string userInput, Station station[], int S, BikeTrips trip[], int T) {
    // looping through once to see if there are any matches
    int count = 0;
    for (int i = 0; i < S; ++i) {
        if (station[i].stationName.find(userInput) != string::npos) {
            count++;
        }
    }

    // if count is empty, cout none
    // found, else, create a new array,
    // fill it with the stations that 
    // contain userInput, then use
    // listAllStations to sort it 
    // alphabetically, then print em
    if (count == 0) {
        cout << "none found" << endl;
    } else {
        // creating a new array to hold the list
        // and looping through again to fill it
        Station* tempL = new Station[count];
        int j= 0;
        for (int i = 0; i < S; ++i) {
            if (station[i].stationName.find(userInput) != string::npos) {
                tempL[j] = station[i];
                j++;
            }
        }
        
        listAllStations(tempL, count, trip, T);
        delete[] tempL;
    }
}

//
// timeConverter
// helper function for tripTimeSpan
// converts given times into the
// minute in the day they correspond to
//

int timeConverter(string time) {
    int ntime = 0;
    int hrs = 0;
    int mins = 0;

    // depending on the string's
    // length, it takes out the
    // hour and minutes,
    // then multilies the hour by 60
    // and then adds the minutes to 
    // get the minute in the day
    if (time.length() == 4) {
        hrs = stoi(time.substr(0,1));
        mins = stoi(time.substr(2,4));
        ntime = (hrs * 60) + mins;
    } else if (time.length() == 5) {
        hrs = stoi(time.substr(0,2));
        mins = stoi(time.substr(3,5));
        ntime = (hrs * 60) + mins;
    }

    return ntime;

}

// binanysearch again but for 
// the tripTimeSpan fucntion
int binarySearch2(Station station[], int N, string key) {
   int mid;
   int low;
   int high;
   
   low = 0;
   high = N - 1;
   
   while (high >= low) {
      mid = (high + low) / 2;
      if (station[mid].stationID < key) {
         low = mid + 1;
      }
      else if (station[mid].stationID > key) {
         high = mid - 1;
      }
      else {
         return mid;
      }
   }
   
   return -1; // not found
}

//
// tripTimeSpan
// searches for all trips within
// a given timespan, then outputs
// the num of trips found, the avg
// duration, and stations where the trips started
//
void tripTimeSpan(string time1, string time2, Station station[], int S, BikeTrips trip[], int T) {
    int count = 0;
    int time[T] = {0};
    int ntime1, ntime2 = 0;


    // so here i'm looping through the startTimes
    // and converting all of them to the minute of
    // the day they represent, and storing them in
    // an array time
    for (int i = 0; i < T; ++i) {
        time[i] = timeConverter(trip[i].startTime);
    }

    // converting time1 and time2 into
    // minute of the day
    ntime1 = timeConverter(time1);
    ntime2 = timeConverter(time2);
    
    for (int i = 0; i < T; ++i) {
        if (ntime1 != ntime2) {
            if (ntime1 > ntime2) {
                if (time[i] >= ntime1) {
                    count++;
                    if (time[i] <= ntime2) {
                        count++;
                    }
                } 
            } else {
                if ((ntime1 <= time[i]) && (time[i] <= ntime2)) {
                        count++;
                    }
            }
        } else if (ntime1 == ntime2) {
            if (time[i] == ntime1) {
                count++;
            }
        }
    }
    
    if (count == 0) {
        cout << "none found" << endl;
    } else {
        BikeTrips* tracker = new BikeTrips[count];
        int j = 0;
        
        for (int i = 0; i < T; ++i) {
            if (ntime1 != ntime2) {
                if (ntime1 > ntime2) {
                    if (time[i] >= ntime1) {
                        tracker[j] = trip[i];
                        j++;
                        if (time[i] <= ntime2) {
                            tracker[j] = trip[i];
                            j++;
                        }
                    } 
                } else {
                    if ((ntime1 <= time[i]) && (time[i] <= ntime2)) {
                            tracker[j] = trip[i];
                            j++;
                        }
                }
            } else if (ntime1 == ntime2) {
                if (time[i] == ntime1) {
                    tracker[j] = trip[i];
                    j++;
                }
            }
        }
        
        int avgDuration = 0;
        
        for (int i = 0; i < count; ++i) {
            avgDuration = avgDuration + (tracker[i].tripDuration / 60);
        }
        avgDuration = avgDuration / count;
        //cout << avgDuration << endl;
        
        int c = 0;
        for (int i = 1; i < count; ++i) {
            if (tracker[i-1].startStation == tracker[i].startStation) {
                continue;
            } else {
                c++;
            }
        }
        
        cout << count << " trips found" << endl;
        cout << "avg duration: " << avgDuration << endl;
        delete[] tracker;
    }// end of if/else
    
}// end of function 


int main() {
    
    ifstream infile;
    string inf1, inf2, command;

    Station* station = nullptr;
    BikeTrips* trip = nullptr;

    int S, T = 0;
    
    

    // prompts the user to input station data
    cout << "Welcome to DIVVYTool!" << endl << "Please enter your station data> ";
    cin >> inf1;
    // prompts the user to input trip data
    cout << "Thanks! Now please enter your trip data> ";
    cin >> inf2;
    cout << "** Inputing DIVVY data..." << endl;

    // putting the station data in an array
    station = inputStationData(inf1, S);
    if (station == nullptr) {
        cout << "Error opening file '" << inf1 << "'. Goodbye. :)";
        return 0;
    }

    // putting the trip data in an array
    trip = inputTripData(inf2, T);
    if (trip == nullptr) {
        cout << "Error opening file '" << inf2 << "'. Goodbye. :)";
        return 0;
    }

    // lists user commands, prompts the user to enter a command
    cout << "** Processing user commands..." << endl;
    cout << "Available commands:" << endl;
    cout << "stats" << endl << "durations" << endl << "starting" << endl << "nearme(lat,long)" << endl << "stations" << endl << "find(name)" << endl << "trips(start, end)" << endl;
    cout << "Enter command (press # to stop, c to view available commands): " << endl;
    cin >> command;

    while (command != "#") {

        // stats branch:
        // outputs stats about stations, trips,
        // and total bike capacity
        if (command == "stats") {
            quickStats(station, S, T);
        } 
        // durations branch:
        // outputs the number of trips in given
        // duration ranges
        else if (command == "durations") {
            tripDurations(trip, T);
        }
        // stations branch:
        // lists all stations in abc order
        // with the name, stationID, position,
        // capacity, and total # of bike trips
        // to or from this station
        else if (command == "stations") {
            Station* tempS = new Station[S];
            
            for (int i = 0; i < S; ++i) {
                tempS[i] = station[i];
            }

            listAllStations(tempS, S, trip, T);
            delete[] tempS;

        }
        // starting branch:
        // outputs the number of trips
        // that started during each hour
        // of the day
        else if (command == "starting") {
            startTimes(trip, T);
        }
        // nearme branch:
        // outputs stations within D miles
        // of a given latitude and longitude
        else if (command == "nearme") {
            // vars for lat, long, and D num of miles
            double lat1, long1, D;
            cin >> lat1;
            cin >> long1;
            cin >> D;
            nearbyStations(station, S, D, lat1, long1);
        }
        // find branch:
        // searches for all stations that
        // have userInput in their name
        else if (command == "find") {
            string userInput;
            cin >> userInput;
            findStations(userInput, station, S, trip, T);
        }
        // trips branch:
        // searches for all trips within
        // a given timespan, the outputs
        // the num of trips found, the avg
        // trip duration, and stations where trips started
        else if (command == "trips") {
            string time1, time2;
            cin >> time1;
            cin >> time2;
            tripTimeSpan(time1, time2, station, S, trip, T);
        }
        // command branch:
        // just lists all available commands
        else if (command == "c") {
            cout << "Available commands:" << endl;
            cout << "stats" << endl << "durations" << endl << "starting" << endl << "nearme(lat,long)" << endl << "stations" << endl << "find(name)" << endl << "trips(start, end)" << endl;
        }
        // invalid command branch:
        else {
            cout << "Invalid command, please try again. " << endl;
        }
        cout << "Enter command (press # to stop, c to view available commands): " << endl;
        cin >> command;
    } // end of while loop
    
    cout << "Have a nice day. :)" << endl;

    // getting rid of the arrays

    delete[] station;
    delete[] trip;

   return 0;
}