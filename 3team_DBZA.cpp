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
            sql::SQLString url = "jdbc:mariadb://LOCALHOST/DBZA";    // db�� �ּ�
            sql::Properties properties({{"user", m_id}, {"password", m_pw}});
            m_connection = driver->connect(url,properties);
            cout << "DB ����\n";  
        }
        catch(sql::SQLException& e)
        {
            cerr<<"DB ���� ����: " << e.what() << endl;
        }
    }
     
    vector<string> local()              // ���� ����Ʈ
    {   
        vector<string> local;
        string name;
        string query = "SELECT NAME FROM LOCATION";
        sql::Statement* loc(m_connection->createStatement());
        sql::ResultSet *resloc = loc->executeQuery(query);
        
        cout << "��  ����ϵ� Ư����ġ�� ���� ���� ���α׷� ��" << endl << endl;
        cout << " | �� ��  �� �� Ʈ" << endl;
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
    
    void F_list(string local)           // ���� ����Ʈ ���
    {
        try
        {
            sql::PreparedStatement* flist = F_db.prepareStatement("select FID, F_NAME from FESTIVAL where NAME = ?");
            flist->setString(1,local);

            sql::ResultSet *res = flist->executeQuery();
            cout <<"\n======== ���� ����Ʈ =======\n" << endl;
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
    
    void F_info(int num)                // ���� ���� ���
    {
        try
        {
            sql::PreparedStatement* finfo =  F_db.prepareStatement("select * from FESTIVAL where FID = ?");
            finfo->setInt(1, num); 
            
            sql::ResultSet *res = finfo->executeQuery();
            cout <<"\n=========== ���� ���� ==========\n" << endl;
            while (res->next()) 
            {
                cout<< "| ������ : " << res->getString(3) << endl;
                cout<< "| ��  �� : " << res->getString(5) << endl;
                cout<< "| ��  �� : " << res->getString(4) << endl;
                cout<< "| ��  �� : " << res->getString(6) << endl;
                cout<< "| ������ �� : " << res->getString(7) << " ��" << endl;
                cout<< "| ��ü��� : " << res->getString(8) << endl;
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
    
    void C_info(string local)           // ķ���� ���� ���
    {
        try
        {
            
            sql::PreparedStatement* camp =  C_db.prepareStatement("select C_NAME, ADDRESS from CAMPING where NAME = ?");
            camp->setString(1, local); 
            
            sql::ResultSet *res = camp->executeQuery();
            cout <<"\n======== ��ó ķ���� �� ���� ���� =======\n" << endl;
            while (res->next()) 
            { 
                cout << " | " << setw(2) << res->getString(1) << endl;
                cout << " | ��  �� : "<< res->getString(2) << endl << endl;
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

    void T_list(string local)           // ������ ����Ʈ ���
    {
        try
        {   sql::PreparedStatement* tlist =  T_db.prepareStatement("select TID , T_NAME from TOUR where NAME = ?");
            tlist->setString(1, local); 
            
            sql::ResultSet *res = tlist->executeQuery();
            cout <<"\n======== ������ ����Ʈ=======\n" << endl;
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

    void T_info(int num)                // ������ ���� ���
    {
        try
        {
            sql::PreparedStatement* tinfo =  T_db.prepareStatement("select * from TOUR where TID = ?");
            tinfo->setInt(1, num); 
            
            sql::ResultSet *res = tinfo->executeQuery();
            cout <<"\n=========== ������ ���� ==========\n" << endl;
            while (res->next()) 
            {
                cout << "| ������ �� : " << res->getString(3) << endl;
                cout << "| ��  �� : " << res->getString(4) << endl;
                cout <<"| ����� : " << res->getString(6) << endl;
                cout << "| ��  �� : " << res->getString(5) << endl;
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

    void S_list(string local)           // Ư��ǰ ����Ʈ ���
    {
        try
        {   sql::PreparedStatement* slist =  S_db.prepareStatement("select SID , S_NAME from SPRODUCT where NAME = ?");
            slist->setString(1, local); 
            
            sql::ResultSet *res = slist->executeQuery();
            cout <<"\n======== Ư��ǰ ����Ʈ =======\n" << endl;
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

    void S_info(int num)                // Ư��ǰ ���� ���
    {
        try
        {
            sql::PreparedStatement* sinfo =  S_db.prepareStatement("select S_NAME, S_INFO from SPRODUCT where SID = ?");
            sinfo->setInt(1, num); 
            
            sql::ResultSet *res = sinfo->executeQuery();
            cout <<"\n=========== Ư��ǰ ���� ==========\n" << endl;
            while (res->next())
            {
                cout<< "| Ư��ǰ �� : " << res->getString(1) << endl;
                cout<< "| ��  �� : " << res->getString(2) << endl;
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
    void showRegion()                   // ���� ����Ʈ ���
    {   
        local = db.local();
    }

    void loginaccount(string id, string pw)     // DB �α��� ����
    {
        db.account(id,pw);
        db.connect();
    }   

    int Choice()                        // ����� ���� �Է�
    {
        int num;
        while(1)
        {
            cout << endl << "���ϴ� �׸��� <��ȣ>�� �Է��ϼ���. : ";
            cin >> num;
             if(cin.fail())
             {
               cin.clear();
               cin.ignore(256, '\n');
               cerr << " <<��ȣ>>�� �Է����ּ���\n"; 
                continue;  
              }
             break; 
        }
        return num;
    }

    bool check(string local1)           // ���� ���� �Ǵ�
    {   
        bool check = true;
        if(find(local.begin(), local.end(), local1) == local.end())
        {
            cout << endl
                 << "=======================================" << endl
                 << "\t      ���� �����Դϴ�.\n"
                 << "=======================================" << endl;
            check = false;
        }
        return check;
    }

    void INFO()                         // ���� �˻�
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
            cout << " �˻� ������ �Է��ϼ���. : ";
            cin >> local1;
            if (check(local1) == false)
            {
                cout << endl;
                continue;
            }
            string choice;
            cout << " 1. ����       2. ������       3. Ư��ǰ       4.������" << endl;
            cout << " ���ϴ� �׸��� �Է��ϼ���. : ";
            cin >> choice;
            while (choice != "")
            {
                int num;
                if (choice == "����" || choice == "1" )
                {
                        Finfo.F_list(local1);
                        num = Choice();
                        Finfo.F_info(num);
                        Cinfo.C_info(local1);
                        break;
                }
                else if (choice == "2" || choice == "������")
                {
                        Tinfo.T_list(local1);
                        num = Choice();
                        Tinfo.T_info(num);
                        Cinfo.C_info(local1);
                        break;
                }
                else if (choice == "3" || choice == "Ư��ǰ")
                {
                        Sinfo.S_list(local1);
                        num = Choice();
                        Sinfo.S_info(num);
                        break;
                } 
                else if (choice == "4" || choice == "������")
                {
                    out = 0;
                    break;
                }
                else
                { 
                    cout << "�ٽ� �Է��ϼ���\n";
                    cout << " 1. ����       2. ������       3. Ư��ǰ       4.������" << endl;
                    cout << " ���ϴ� �׸��� �Է��ϼ���. : ";
                    cin >> choice;
                }
            }
            if (out == 0) { cout << "����"; break; }                                
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