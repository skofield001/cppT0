#ifndef COMMON_INCLUDE_H
#define COMMON_INCLUDE_H
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <map>
#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include "stock_ban_list.h"
#include <algorithm>

const std::string stk_h5_dir = "/t0/tk_arr_h5/arr_h5_files/";
const std::string results_dir = "/t0/cpp_t0/results/";
const std::string factor_compose_dir = results_dir + "factor_compose/";
const std::string ols_results_dir = results_dir + "ols_results/";
const std::string paraOpt_results_dir = results_dir + "paraOpt_results/";
const std::string tc_opt_results_dir = results_dir + "tc_opt_results/";
const std::string t0_finalchoose_dir = results_dir + "choose_final/t0/";
const std::string tc_finalchoose_dir = results_dir + "choose_final/tc/";
const std::string ols_logs_dir = "/t0/cpp_t0/logs/ols_logs/";
const std::string paraOpt_logs_dir = "/t0/cpp_t0/logs/paraOpt_logs/";
const std::string tc_opt_logs_dir = "/t0/cpp_t0/logs/tc_opt_logs/";
const std::string stk_daily_price_csv = "/t0/cpp_t0/results/daily_price/stock_trading_price.csv";
const std::string common_h5_key = "arr";
const std::vector<std::string> h5_all_colnames={"date", "time", "last", "vol", "amount", "ask1", "ask2", "ask3", "ask4",
                                        "ask5", "ask6", "ask7", "ask8", "ask9", "ask10", "ask_vol1", "ask_vol2",
                                        "ask_vol3", "ask_vol4", "ask_vol5", "ask_vol6", "ask_vol7", "ask_vol8",
                                        "ask_vol9", "ask_vol10", "bid1", "bid2", "bid3", "bid4", "bid5", "bid6",
                                        "bid7", "bid8", "bid9", "bid10", "bid_vol1", "bid_vol2", "bid_vol3",
                                        "bid_vol4", "bid_vol5", "bid_vol6", "bid_vol7", "bid_vol8", "bid_vol9",
                                        "bid_vol10", "mid", "L5_ret", "L10_ret", "L20_ret", "L50_ret",
                                        "L100_ret", "L200_ret", "L300_ret", "L400_ret", "IF_last", "IC_last",
                                        "IH_last"};

const int last_col = 2;
const int vol_col = 3;
const int amount_col = 4;
const int mid_col = 45;
const int if_col = 54;
const int ic_col = 55;
const int ih_col = 56; 
const std::vector<int> ask_cols = {5,6,7,8,9,10,11,12,13,14};
const std::vector<int> ask_vol_cols = {15,16,17,18,19,20,21,22,23,24};
const std::vector<int> bid_cols = {25,26,27,28,29,30,31,32,33,34};
const std::vector<int> bid_vol_cols = {35,36,37,38,39,40,41,42,43,44};
const std::vector<int> ret_cols = {46,47,48,49,50,51,52,53};
const std::vector<int> ret_intervals = {5,10,20,50,100,200,300,400};
const std::vector<int> spread_intervals = ret_intervals;

template<typename T>
inline bool IsValid(const T &n)
{return !std::isnan(n);}

template<typename T>
inline double sign(const T &n) 
{
  if(std::fabs(n)<1e-6){return 0;}
  if(n>0){return 1;}
  if(n<0){return -1;}
  return 0;
}

inline double vec_mean(const std::vector<double> &input_vec)
{
  if(input_vec.size()<1){return 0;} 
  double sumx = 0.0;
  double count = 0.0;
  for(auto &r:input_vec)
  {
    if(IsValid(r))
    {   
      sumx += r;
      count ++;
    }   
  }
  if(count<1){return 0;} 
  return sumx/count;
}


inline double vec_std(const std::vector<double> &input_vec)
{
  if(input_vec.size()<2){return 0;}
  double sumx = 0.0;
  for(auto &r:input_vec)
  {
    if(IsValid(r))
    {
      sumx += r;
    }
  }
  double meanx = sumx/input_vec.size();
  double sumx_mean = 0;
  double count = 0;
  for(auto &r:input_vec)
  {
    if(IsValid(r))
    {
     sumx_mean += (r-meanx)*(r-meanx);
     count ++;
    }
  }
  if(count<1){return 0;}
  return std::sqrt(sumx_mean/(count-1));
}

inline double vecs_corr(const std::vector<double> &vec1,const std::vector<double> &vec2)
{
  if(vec1.size()!=vec2.size()||vec1.size()<2){return 0;}
  double sumx = 0;
  double sumy = 0;
  double count = 0;
  for(size_t i=0;i<vec1.size();i++)
  {
    if(IsValid(vec1[i])&&IsValid(vec2[i]))
    {
      sumx += vec1[i];
      sumy += vec2[i];
      count ++;
    }
  }
    
  double meanx = sumx/count;
  double meany = sumy/count;
    
  double fenzi = 0;
  double fenmu1 = 0;
  double fenmu2 = 0;
  for(size_t i=0;i<vec1.size();i++)
  {
    if(IsValid(vec1[i])&&IsValid(vec2[i]))
    {    
      fenzi += (vec1[i]-meanx)*(vec2[i]-meany);
      fenmu1 += (vec1[i]-meanx)*(vec1[i]-meanx);
      fenmu2 += (vec2[i]-meany)*(vec2[i]-meany);
    }
  }
  double fenmu = std::sqrt(fenmu1*fenmu2);
  if(std::fabs(fenmu)<1e-6){return 0;}
  return fenzi/fenmu;
}


inline double vec_skew(const std::vector<double> &input_vec)
{ 
  try
  {
    double n = double(input_vec.size());
    if(n<2){return 0;}
    double sum1 = 0.0;
    double sum2 = 0.0;
    double m = vec_mean(input_vec);
    for(size_t i=0;i<n;i++)
    {
      if(IsValid(input_vec[i]))
      {
        sum1 += std::pow(input_vec[i]-m,3);
        sum2 += std::pow(input_vec[i]-m,2);
      }
    }
    return (std::pow(n*(n-1),0.5)) / (n-2) * (sum1/n) / std::pow(sum2/n,1.5);
  }
  
  catch(const std::exception& e)
  {
    printf("%d,%s", __LINE__, e.what());
    return 0.0;
  }
}


inline double vec_kurt(const std::vector<double> &input_vec)
{ 
  try
  {
    double n = double(input_vec.size());
    if(n<3){return 0;} 
    double sum = 0.0;
    double m = vec_mean(input_vec);
    double std = vec_std(input_vec);

    for(size_t i=0;i<n;i++)
    {
      if(IsValid(input_vec[i]))
      {   
        sum += std::pow((input_vec[i]-m)/std,4);
      }   
    }
    return n*(n+1)/(n-1)/(n-2)/(n-3)*sum - 3*std::pow(n-1,2)/(n-2)/(n-3);
  }

  catch(const std::exception& e)
  {
    printf("%d,%s", __LINE__, e.what());
    return 0.0;
  }
}


std::vector<std::string> get_list_from_txt(const std::string &filepath)
{
  std::ifstream infile;
  infile.open(filepath);
  std::string s;
  std::vector<std::string> list_vec;
  while(getline(infile,s))
  {  
    if(s.size()>3)
    list_vec.push_back(s);
  }
  infile.close();
  return list_vec;
}


std::string RemoveAllSpace(std::string str)
{
  str.erase(remove(str.begin(), str.end(), ' '), str.end());
  return str;
}


std::vector<std::string> get_factorlist_by_compose_name(const std::string &factor_compose_name)
{
  std::string factor_compose_txt = factor_compose_dir + factor_compose_name + ".txt";
  return get_list_from_txt(factor_compose_txt);
}


inline bool IsDirExist(const char* path) 
{
  DIR* dp;
  if ((dp = opendir(path)) == nullptr) 
    return false;
  closedir(dp);
  return true;
}

inline bool IsFileExist(const std::string &filepath)
{
    std::fstream fs;
    fs.open(filepath,std::ios::in);
    if(fs)
    {
        fs.close();
        return true;
    }
    return false;
}


inline std::string get_cur_time()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [20];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime (buffer,20,"%Y%m%d %H%M%S",timeinfo);
    std::string time_str = buffer;
    return time_str;
}


inline std::string Trim(std::string &str)
{
  str.erase(0,str.find_first_not_of(" \t\r\n"));
  str.erase(str.find_last_not_of(" \t\r\n")+1);
  return str;
}


inline std::map<int,std::map<std::string,double>> get_ols_para(const std::string &stk,const std::string factor_compose_name)
{
  std::map<int,std::map<std::string,double>> para_map;   // <interval,<(factorname rsquare or samplesize),value>>  
  std::string csv_filepath = ols_results_dir + factor_compose_name + "/" + stk + ".csv";
  if(IsFileExist(csv_filepath))
  {
    std::ifstream infile(csv_filepath,std::ios::in);
    std::string line_str;
    std::vector<std::vector<std::string>> csv_vec;
    while(getline(infile,line_str))
    {
      std::vector<std::string> line_vec;
      std::stringstream ss(line_str);
      std::string str;
      while(getline(ss,str,'#'))
        line_vec.push_back(Trim(str));
      csv_vec.push_back(line_vec);
    }

    if(csv_vec.size()>1)
    {
      for(size_t i=1;i<csv_vec.size();i++)
      {
        if(csv_vec[i].size()==csv_vec[0].size())
        {
          for(size_t j=1;j<csv_vec[0].size();j++)
          {
            para_map[std::stoi(csv_vec[i][1])][RemoveAllSpace(csv_vec[0][j])] = std::stod(csv_vec[i][j]);
          } 
        }
      }
    } 
   }
   return para_map;
}
   
  
inline std::vector<std::string> get_StkList_from_h5Folder(const std::string &h5_folder = "/t0/tk_arr_h5/arr_h5_files/")
{
  DIR *d = opendir(h5_folder.c_str());
  if(d==NULL)
  {
    perror("open dir error");
    exit(1);
  }
  std::vector<std::string> stock_list;
  struct dirent* entry;
  while((entry=readdir(d))!=NULL)
  {
    std::string filename = entry->d_name;
    if(filename.size()==12) {stock_list.push_back(filename.substr(0,9));}
  } 
  closedir(d);
  return stock_list;
}
  
    
inline void logWriter(const std::string &log_file,const std::string &log_content)
{
  std::fstream fp(log_file,std::ios::app);
  fp<<log_content;
  fp.close();
}    


template<typename T>
inline int sgn(T val)
{
  return (T(0)<val) - (val<T(0));
}


template<typename T>
inline bool is_element_in_vector(const std::vector<T> &vec,const T& element)
{
  auto it = std::find(vec.begin(),vec.end(),element);
  if(it!=vec.end()) {return true;}
  else {return false;}
}


inline std::vector<std::string> get_NoBan_stk_list()
{
  std::vector<std::string> noban_stk_list;
  std::vector<std::string> all_stk_list = get_StkList_from_h5Folder();
  for(auto &r:all_stk_list)
  {
    if(!is_element_in_vector(stock_ban_list,r)) {noban_stk_list.push_back(r);}
  }
  return noban_stk_list;
}




  
#endif































