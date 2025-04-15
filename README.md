# CS314-Project - Chocoholics Anonymous

## **Core Functionality**
---
### Provider Terminal
```
The Provider Terminal allows Service providers to interact with the ChocAn Management System.
Through the terminal, Providers are able to charge ChocAn for services provided
to qualifying ChocAn Members.
```
#### **Login:** Provider enters their 9-digit provider number.
#### **Member Validation:**
1.  Provider 'swipes' member card 
	  - Enter 9-Digit member number
2. System validates number and status (active/suspended).
	  - Active : Bills Paid, all is normal
	- Member Suspended : Bills unpaid, Suspended until Payment
#### **Service Billing:**
1. **Re-Validate** member number.
2. Input **date of service** (MM-DD-YYYY).
3. Input **6-digit service code**
4. Display and confirm service        
5. Optionally enter **comments** (max 100 characters).
6. Display **fee** for the selected service.
7. Save a **record** with:
	- Current date and time (MM-DD-YYYY HH:MM:SS)
	- Date of service
	- Provider number
	- Member number
        - Service code
        - Previously supplied Optional Comment

#### Provider Directory
On request, generate a file with:
- Alphabetical list of service names
- Corresponding 6-digit service codes
- Associated service fees

---
### Manager / Admin Terminal
```
The Admin terminal is in charge of managing Member and Provider Data.
The Admin Terminal managers all member and provider information and history.
Through the admin terminal, we can also generate reports based on the data
sent from our Providers throughout the week as well as general information
given to us by our clients.
```
#### Member & Provider Record Management (Admin Tasks)
- Add, delete, update **members**  
- Add, delete, update **providers**

#### **Weekly Accounting Process (Midnight Friday or on-demand)**
##### Member Report & Provider Report
```
/Member_Reports/MemberName_MM-DD-YYYY.txt
/Provider_Reports/ProviderName_MM-DD-YYYY.txt

FILENAME FORMAT:
	Member   : MemberName_MM-DD-YYYY.txt
	Provider : ProviderName_MM-DD-YYYY.txt

Reports are saved as files rather than actual email.
```
For each active member who received services:
- Name(25)
- Member/Provider Number(9)
- Street Address(25) 
- City(14)
- State(2)
- Zip Code(5)
- **Member** Additional Service Information:
	- Date (MM-DD-YYYY)
	- Provider Name(25)
	- Service Name(20)
- **Provider** Additional Service Information:
	- Date (MM-DD-YYYY)
	- Date-Time Received (MM-DD-YYYY HH:MM:SS)
	- Member Name
	- Member Number
   	- Service Code
        - Fee

### Manager Summary Report
```
Provider Reports are processed to give the manager a summary for all accounts payable.
```
- All providers to be paid that week:
    - Provider Name(25)
    - Number of Consultations (3)
    - Total Fee ($99,999.99 MAX)
- Print:
    - Total Number of Providers who provided Service
    - Total Number of Consultations
    - Overall Fee Total

### EFT Data File
For each provider to be paid:
- Provider Name(25)
- Provider Number(9)
- Total Fee($99,999.99 MAX)
