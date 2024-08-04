#include <iostream>
#include <mariadb/conncpp.hpp>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;

class DB
{
private:

    string m_id;
    string m_pw;
    sql::Connection* m_connection;

public:
    DB() { }

    void account(const string& id, const string& pw)       
    {
        m_id = id;
        m_pw = pw;
    }

    sql::PreparedStatement* prepareStatement(const string& query)
    {
        sql::PreparedStatement* stmt(m_connection->prepareStatement(query));
        return stmt;
    }
    
    void connect()
    {
        try
        {
            sql::Driver* driver = sql::mariadb::get_driver_instance();
            sql::SQLString url = "jdbc:mariadb://LOCALHOST/DBZA";    // db의 주소
            sql::Properties properties({{"user", m_id}, {"password", m_pw}});
            m_connection = driver->connect(url,properties);
            cout << "DB 접속\n";  
        }
        catch(sql::SQLException& e)
        {
            cerr<<"DB 접속 실패: " << e.what() << endl;
        }
    }
     
    vector<string> local()              // 지역 리스트
    {   
        vector<string> local;
        string name;
        string query = "SELECT NAME FROM LOCATION";
        sql::Statement* loc(m_connection->createStatement());
        sql::ResultSet *resloc = loc->executeQuery(query);
        
        cout << "【  전라북도 특별자치도 관광 정보 프로그램 】" << endl << endl;
        cout << " | 지 역  리 스 트" << endl;
        int count = 0;
        while(resloc->next())
        {
            count += 1;
            if(count == 8)
                cout << endl;
            cout << "  - "<< resloc->getString(1)<< " " ;
            name = resloc->getString(1);
            local.push_back(name);
        }
        cout<< endl << endl;
        return local;
    }

    ~DB() { cerr<< "bye~\n";}
};

class Festival
{
private:

    DB& F_db;

public:
    Festival(DB& fdb) : F_db(fdb) {}
    
    void F_list(string local)           // 축제 리스트 출력
    {
        try
        {
            sql::PreparedStatement* flist = F_db.prepareStatement("select FID, F_NAME from FESTIVAL where NAME = ?");
            flist->setString(1,local);

            sql::ResultSet *res = flist->executeQuery();
            cout <<"\n======== 축제 리스트 =======\n" << endl;
            while (res->next()) 
            {  
                cout << setw(2) << res->getInt(1) << " | "  
                    << res->getString(2) << endl; 
            }
        
        }
        catch(sql::SQLException& e)
        {
            cerr << "Error: " <<e.what() << endl;
        }
    }
    
    void F_info(int num)                // 축제 정보 출력
    {
        try
        {
            sql::PreparedStatement* finfo =  F_db.prepareStatement("select * from FESTIVAL where FID = ?");
            finfo->setInt(1, num); 
            
            sql::ResultSet *res = finfo->executeQuery();
            cout <<"\n=========== 축제 정보 ==========\n" << endl;
            while (res->next()) 
            {
                cout<< "| 축제명 : " << res->getString(3) << endl;
                cout<< "| 기  간 : " << res->getString(5) << endl;
                cout<< "| 유  형 : " << res->getString(4) << endl;
                cout<< "| 장  소 : " << res->getString(6) << endl;
                cout<< "| 관광객 수 : " << res->getString(7) << " 명" << endl;
                cout<< "| 주체기관 : " << res->getString(8) << endl;
            }
        }
        catch(sql::SQLException& e)
        {
            cerr << "Error: " << e.what() << endl;
        }
    }
};

class Camping
{
private: 
    DB& C_db;
public:
    Camping(DB& cdb) : C_db(cdb) {}
    
    void C_info(string local)           // 캠핑장 정보 출력
    {
        try
        {
            
            sql::PreparedStatement* camp =  C_db.prepareStatement("select C_NAME, ADDRESS from CAMPING where NAME = ?");
            camp->setString(1, local); 
            
            sql::ResultSet *res = camp->executeQuery();
            cout <<"\n======== 근처 캠핑장 및 숙소 정보 =======\n" << endl;
            while (res->next()) 
            { 
                cout << " | " << setw(2) << res->getString(1) << endl;
                cout << " | 주  소 : "<< res->getString(2) << endl << endl;
            }
            cout << endl << endl;
        } 
        catch(sql::SQLException& e)
        {
        cerr << "Error: " << e.what() << endl;
        }
    }

};

class Tour
{
private:
    DB& T_db;
public: 
    Tour(DB& tdb):T_db(tdb) {}

    void T_list(string local)           // 관광지 리스트 출력
    {
        try
        {   sql::PreparedStatement* tlist =  T_db.prepareStatement("select TID , T_NAME from TOUR where NAME = ?");
            tlist->setString(1, local); 
            
            sql::ResultSet *res = tlist->executeQuery();
            cout <<"\n======== 관광지 리스트=======\n" << endl;
            while (res->next()) 
            { 
                 cout << setw(2) << res->getInt(1) << " | "  
                      << res->getString(2) << endl; 
            }
            cout << endl << endl;
        } 
        catch(sql::SQLException& e)
        {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void T_info(int num)                // 관광지 정보 출력
    {
        try
        {
            sql::PreparedStatement* tinfo =  T_db.prepareStatement("select * from TOUR where TID = ?");
            tinfo->setInt(1, num); 
            
            sql::ResultSet *res = tinfo->executeQuery();
            cout <<"\n=========== 관광지 정보 ==========\n" << endl;
            while (res->next()) 
            {
                cout << "| 관광지 명 : " << res->getString(3) << endl;
                cout << "| 주  소 : " << res->getString(4) << endl;
                cout <<"| 입장료 : " << res->getString(6) << endl;
                cout << "| 설  명 : " << res->getString(5) << endl;
            }
            cout << endl << endl;
        }
        catch(sql::SQLException& e)
        {
            cerr << "Error: " << e.what() << endl;
        }
    }
};

class Sproduct
{
private:
    DB& S_db;
public: 
    Sproduct(DB& sdb):S_db(sdb) {}

    void S_list(string local)           // 특산품 리스트 출력
    {
        try
        {   sql::PreparedStatement* slist =  S_db.prepareStatement("select SID , S_NAME from SPRODUCT where NAME = ?");
            slist->setString(1, local); 
            
            sql::ResultSet *res = slist->executeQuery();
            cout <<"\n======== 특산품 리스트 =======\n" << endl;
            while (res->next()) 
            { 
                 cout << setw(2) << res->getInt(1) << " | "  
                      << res->getString(2) << endl; 
            }
            cout << endl << endl;
        } 
        catch(sql::SQLException& e)
        {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void S_info(int num)                // 특산품 정보 출력
    {
        try
        {
            sql::PreparedStatement* sinfo =  S_db.prepareStatement("select S_NAME, S_INFO from SPRODUCT where SID = ?");
            sinfo->setInt(1, num); 
            
            sql::ResultSet *res = sinfo->executeQuery();
            cout <<"\n=========== 특산품 정보 ==========\n" << endl;
            while (res->next())
            {
                cout<< "| 특산품 명 : " << res->getString(1) << endl;
                cout<< "| 설  명 : " << res->getString(2) << endl;
            }
            cout << endl << endl;
        }    
        catch(sql::SQLException& e)
        {
            cerr << "Error: " << e.what() << endl;
        }
    }
};

class Handler
{
private:
    DB db;
    vector<string> local;
public:
    void showRegion()                   // 지역 리스트 출력
    {   
        local = db.local();
    }

    void loginaccount(string id, string pw)     // DB 로그인 접속
    {
        db.account(id,pw);
        db.connect();
    }   

    int Choice()                        // 사용자 선택 입력
    {
        int num;
        while(1)
        {
            cout << endl << "원하는 항목의 <번호>를 입력하세요. : ";
            cin >> num;
             if(cin.fail())
             {
               cin.clear();
               cin.ignore(256, '\n');
               cerr << " <<번호>>로 입력해주세요\n"; 
                continue;  
              }
             break; 
        }
        return num;
    }

    bool check(string local1)           // 전북 지역 판단
    {   
        bool check = true;
        if(find(local.begin(), local.end(), local1) == local.end())
        {
            cout << endl
                 << "=======================================" << endl
                 << "\t      없는 지역입니다.\n"
                 << "=======================================" << endl;
            check = false;
        }
        return check;
    }

    void INFO()                         // 정보 검색
    {    
        Festival Finfo(db);
        Camping Cinfo(db);
        Tour Tinfo(db);
        Sproduct Sinfo(db);
        string local1;          
        while(1)
        {
            int out = 1;    
            showRegion();
            cout << " 검색 지역을 입력하세요. : ";
            cin >> local1;
            if (check(local1) == false)
            {
                cout << endl;
                continue;
            }
            string choice;
            cout << " 1. 축제       2. 관광지       3. 특산품       4.나가기" << endl;
            cout << " 원하는 항목을 입력하세요. : ";
            cin >> choice;
            while (choice != "")
            {
                int num;
                if (choice == "축제" || choice == "1" )
                {
                        Finfo.F_list(local1);
                        num = Choice();
                        Finfo.F_info(num);
                        Cinfo.C_info(local1);
                        break;
                }
                else if (choice == "2" || choice == "관광지")
                {
                        Tinfo.T_list(local1);
                        num = Choice();
                        Tinfo.T_info(num);
                        Cinfo.C_info(local1);
                        break;
                }
                else if (choice == "3" || choice == "특산품")
                {
                        Sinfo.S_list(local1);
                        num = Choice();
                        Sinfo.S_info(num);
                        break;
                } 
                else if (choice == "4" || choice == "나가기")
                {
                    out = 0;
                    break;
                }
                else
                { 
                    cout << "다시 입력하세요\n";
                    cout << " 1. 축제       2. 관광지       3. 특산품       4.나가기" << endl;
                    cout << " 원하는 항목을 입력하세요. : ";
                    cin >> choice;
                }
            }
            if (out == 0) { cout << "종료"; break; }                                
        }
    }   
};

int main()
{
    string id = "OPERATOR";
    string pw = "1234";
    Handler user;
    user.loginaccount(id,pw);
    user.INFO();
    return 0;
}