//Markov Evgenii  Student number:	249560
#include <string>
#include <iostream> 
#include <vector> 
#include <algorithm> //sort by
 
using namespace std;
 

 
const int SIZE = 16;
 
const string STARTENDLOCATIONS[SIZE] = {
        "hervanta:mikontalo",
        "mikontalo:orivedenkatu",
        "orivedenkatu:turtola",
        "turtola:keskustori",
        "mikontalo:orivedenkatu",
        "orivedenkatu:vaajakatu",
        "vaajakatu:toas",
        "toas:keskustori",
        "hermia:tty",
        "tty:lukonmaenkatu",
        "lukonmaenkatu:kivikirkko",
        "kivikirkko:tampere-talo",
        "tampere-talo:railwaystation",
        "railwaystation:keskustori",
        "keskustori:rauhantie",
        "rauhantie:ylojarvi-matkatie"
};
 
const int BUSNUMBERS[SIZE] = { 3, 3, 3, 3, 4, 4, 4, 4, 13, 13, 13, 13, 13, 13, 13, 13 };
 
const int STARTTIMINGHOURS[SIZE] = { 9, 9, 9, 9, 9, 9, 9, 9, 8, 8, 8, 8, 9, 9, 9, 9 };
const int STARTTIMINGMINUTES[SIZE] = { 0, 15, 17, 20, 15, 17, 20, 35, 10, 15, 25, 55, 3, 10, 20, 55 };
 
const int DESTINATIONTIMINGHOURS[SIZE] = { 9, 9, 9, 9, 9, 9, 9, 9, 8, 8, 8, 9, 9, 9, 9, 10 };
const int DESTINATIONTIMINGMINUTES[SIZE] = { 15, 17, 20, 35, 17, 20, 35, 50, 15, 25, 55, 3, 10, 20, 55, 10 };
 
const string HOURS_ERROR = "Error: Entered Hour is not in the range!";
const string MINUTES_ERROR = "Error: Entered Minutes is not in the range!";
 
// ******************** This part cannot be edited ***************************/
 
const int HOURS_IN_DAY = 24; //Defines constants for validations input
const int MINUTES_IN_HOUR = 60;
 
const char LOCATION_DELIMETR = ':';
 
//The structure for storing a pair of hour, minute
struct DayTime  //Time in the structure DayTime( hour +minutes)
{
        DayTime() {}
 
        DayTime(int h, int min)
        {
                hour = h;
                minutes = min;
        }
 
        int hour;
        int minutes;
 
        //Refund the difference between the time in minutes
        int diff(const DayTime& rhs) const
        {
                return ((hour - rhs.hour) * MINUTES_IN_HOUR + (minutes - rhs.minutes));
        }
};
 
struct TimetableEntry
{
        int busNumber; 
        string startLocation;
        string endLocation;
        DayTime startTiming;
        DayTime destinationTiming;
 
        void setStartAndEndLocations(const string& str)
        {
                int delimPosition = str.find(LOCATION_DELIMETR);//Find  ":"
                startLocation = str.substr(0, delimPosition);
                endLocation = str.substr(delimPosition + 1, str.length());
        }
 
        void show() const //Print to the console
        {
                cout << "Bus Number " << busNumber <<
                        " starts at " << startTiming.hour << ":" << startTiming.minutes <<
                        " from '" << startLocation << "' goes to '" << endLocation <<
                        "' by " << destinationTiming.hour << ":" << destinationTiming.minutes;
        }
};
 
bool checkHour(int hour)
{
        if (hour < 0 || hour >= HOURS_IN_DAY)//Check hour
        {
                cout << HOURS_ERROR;
                return false;
        }
        return true;
}
 
bool checkMinutes(int minutes)
{
        if (minutes < 0 || minutes >= MINUTES_IN_HOUR)
        {
                cout << MINUTES_ERROR;
                return false;
        }
        return true;
}
 
 
bool compareTimetableEntries(const TimetableEntry& lhs, const TimetableEntry& rhs)
{
        return (lhs.startTiming.diff(rhs.startTiming) < 0); //A comparison of the time of departure
}
 
 

vector<TimetableEntry> parseInput()
{
        vector<TimetableEntry> timetable;
        timetable.reserve(SIZE);
        for (int i = 0; i < SIZE; ++i)
        {
                TimetableEntry entry = {};             //Assign all
                entry.busNumber = BUSNUMBERS[i];
                entry.setStartAndEndLocations(STARTENDLOCATIONS[i]);
                entry.startTiming = DayTime(STARTTIMINGHOURS[i], STARTTIMINGMINUTES[i]);
                entry.destinationTiming = DayTime(DESTINATIONTIMINGHOURS[i], DESTINATIONTIMINGMINUTES[i]);
                timetable.push_back(entry);
        }
        return timetable;
}
 
void solve(const vector<TimetableEntry>& sortedTimetable, const DayTime& targetTime)
{
        int closestIndex = 1;
        int entriesCount = 1;
        int closestDiff = INT_MAX;//Maximal int for compare
        for (int i = 0; i < SIZE; ++i)
        {
                int diff = sortedTimetable[i].startTiming.diff(targetTime);
                if (diff >= 0 && diff < closestDiff) //Found a more appropriate time
                {
                        entriesCount = 1;
                        closestIndex = i;
                        closestDiff = diff;
                }
                else if (diff == closestDiff) //Repeating
                {
                        ++entriesCount;
                }
        }
 
        if (closestDiff != INT_MAX) 
        {
                for (int i = closestIndex; i < closestIndex + entriesCount; ++i)
                {
                        sortedTimetable[i].show();
                        cout << endl;
                }
        }
}
 
int main()
{
        int hour;
        int minutes;
 
        cout << "Enter the Hour: ";
        cin >> hour;
        if (!checkHour(hour))
        {
                return NULL;
        }
 
        cout << "Enter the Minutes: ";
        cin >> minutes;
        if (!checkMinutes(minutes))
        {
                return -1;
        }
        DayTime targetTime(hour, minutes);
 
        
        vector<TimetableEntry> timetable = parseInput();
        sort(timetable.begin(), timetable.end(), compareTimetableEntries); 
 
        solve(timetable, targetTime);
        return 0;
}