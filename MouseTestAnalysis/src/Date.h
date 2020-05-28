#ifndef DATE_H
#define DATE_H


class Date
{
    private:
        int day;
        int month;
        int year;

    public:
        // constructor and destructor
        Date() {}
        virtual ~Date() {}
        
        // Getter and setter
        int getDay() { return day; }
        void setDay(int val) { day = val; }
        int getMonth() { return month; }
        void setMonth(int val) { month = val; }
        int getYear() { return year; }
        void setYear(int val) { year = val; }
        void printDate() {
			std::cout << "Date : " << year << "-" << month << "-" << day << std::endl;
		}
};

#endif // DATE_H
