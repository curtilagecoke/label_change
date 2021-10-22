#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<iomanip>
#include<fstream>
#include<typeinfo> 
#include<map>
#include<tr1/unordered_map>
#include<vector>
#include<algorithm>
#include<iomanip>
using namespace std;
using namespace std::tr1;
vector<string>byte{"千","百","十",""};
vector<string>value{"零","一","二","三","四","五","六","七","八","九"};
bool next_dig=0;
string transform_four(string dig_str){//针对有4位数字的情况，专门封装处理。
    //针对中文语法，出现连续相隔1位及以上的0，这时候只会补上一个零字。
    string ans="";
    bool zero=false;//中文语法中的问题，类似于2002，读错两千零二，这个zero标记用以标记零这个字如何使用。
    if(dig_str[0]!='0'){
        ans+=value[dig_str[0]-'0'];//如果这时候首位不为0，加上这个首位值所映射到的中文。
        ans+=byte[0];//因为确定是4位，所以可以带上单位千。
        zero=false;
    }    
    if(dig_str[0]=='0'){//多余的一个判断，因为都是4位数字不会出现首位为0的情况了，没有意义。但是为了保证程序的正确性，勿删。
        ans+=value[0];
        zero=true;
    }
    if(dig_str[1]!='0'){
        ans+=value[dig_str[1]-'0'];//如果这时候首位不为0，加上这个首位值所映射到的中文。
        ans+=byte[1];//因为确定是第3位，所以可以带上单位百。
        zero=false;//出现0的标记可以终止。
    }
    if(dig_str[1]=='0'){
        if(!zero){
            ans+=value[0];//如果这时候是0且零标记为false时，我们要加上一个零字，然后把零标记置为true。
            zero=true;
        }
    }
    if(dig_str[2]!='0'){
        //在4位阿拉伯数字的中文语法中，不会出现2010为两千零十的说法，而是两千零一十，所以这里要注意补上它的权值
		    ans+=value[dig_str[2]-'0'];//如果这时候首位不为0，加上这个首位值所映射到的中文。
        ans+=byte[2];//因为确定是第2位，所以可以带上单位十。
        zero=false;
    }
    if(dig_str[2]=='0')
    {
        if(!zero){
            ans+=value[0];//同上标记的处理
            zero=true;
        }
    }
    if(dig_str[3]!='0'){
        ans+=value[dig_str[3]-'0'];//非零直接加上权值，因为是个位已经没有单位了。
    }
    if(dig_str[3]=='0'){
        if(zero){//如果这时候个位还是为0，那么前面如果有零用来分隔两个不相邻位，这时候需要把这个尾零踢掉，因为一个中文占3个字符位，所以需要pop_back()3次
            ans.pop_back();
            ans.pop_back();
            ans.pop_back();
        }
    }
    return ans;     
}
string change_char(string dig_str,bool last,int k){//最后一个参数意义不明，但可以无视，传进来的参数运作都是在k=0的情况下的
    string ans="";//翻译出来的文本。
    bool zero=false;//中文语法中的问题，类似于2002，读错两千零二，这个zero标记用以标记零这个字如何使用。
    if(!last){//不是最后一段数字的话，一定是4位的，所以可以用专门处理4位的方式来做处理
        ans=transform_four(dig_str);//4位的封装了函数处理
    }else{
        if(k>0){
            if(dig_str.size()==1){
                ans+=value[dig_str[0]-'0'];
            }else{
                if(dig_str[0]!='0'){
                    if(dig_str[0]!='1'){
                        ans+=value[dig_str[0]-'0'];
                    }
                    ans+=byte[2];
                }
                if(dig_str[1]!='0'){
                    ans+=value[dig_str[1]-'0'];
                }
            }//k=0部分可以无视，无意义的功能，应该是当时构建函数的时候有其他想法临时加上的，不影响使用。
        }else{
            if(dig_str.size()==1){//按照位数来处理，因为是最后一段数字，有可能是1位，2位，3位，4位，这里是处理1位数
                ans+=value[dig_str[0]-'0'];
            }
            if(dig_str.size()==2){//这里是处理2位数
                if(dig_str[0]!='0'){
                    if(dig_str[0]!='1'){
                        ans+=value[dig_str[0]-'0'];
                    }
                    ans+=byte[2];//考虑到十比较特别的，数字10~19念作十到十九，不需要带一，所以判断它十位非1的时候才需要加上权值
                }
                if(dig_str[1]!='0'){
                    ans+=value[dig_str[1]-'0'];
                }
            }
            if(dig_str.size()==3){//这里是处理3位数
                ans+=value[dig_str[0]-'0'];
                ans+=byte[1];
                if(dig_str[1]!='0'){
			        ans+=value[dig_str[1]-'0'];
                    ans+=byte[2];
                }
                if(dig_str[1]=='0')
                {
                    ans+=value[0];
                    zero=true;
                }
                if(dig_str[2]!='0'){
                    ans+=value[dig_str[2]-'0'];
                }
                if(dig_str[2]=='0'){
                    if(zero){
                        ans.pop_back();
                        ans.pop_back();
                        ans.pop_back();
                    }
                }
            }
            if(dig_str.size()==4){
                ans=transform_four(dig_str);//4位的封装好了。
            }
        }
    }
    return ans;
}
void build_change(string st,vector<string> &label){
    const string ll1="×",ll2="÷",ll3="＋",ll4="－";//几个特殊字符要进行特别处理，特别是有的是占3个字符位，而有些是2个。
    for(int i=0;i<st.size();i++){
        if(int(st[i])<0){//如果他这时候的ascii码是负数，说明他单字节无法接收这个字符，那么他会溢出变为负数，那么就可以判断他是特殊符号or中文
            string t=st.substr(i,2);//先做特殊符号的判别，因为"×","÷"都是2字节位，所以专门处理。
            if(t==ll1){//截取出来的=×
              label.push_back("乘");
              i+=1;
              continue;
            }
            if(t==ll2){//截取出来的=÷
              label.push_back("除");
              i+=1;
              continue;
            }//2字节的特殊符号处理完了，现在处理3字节位的中文与全角的"+"与"-"
            t=st.substr(i,3);
            if(t!="，"&&t!="。"&&t!="！"&&t!="？"&&t!="、"&&t!="；"){//非这些无用符号的，才进入处理，这里可以再添加过滤掉的符号
                if(t==ll3){
                    label.push_back("加");
                    i+=2;
                    continue;
                }//如果是＋与－单独处理。
                if(t==ll4){
                    label.push_back("减");
                    i+=2;
                    continue;
                }
                label.push_back(t);//否则就是直接截取的就是中文字符，那么直接加入这个中文字就好。
            }
            i+=2;
        }else{//如果是数字或英文
            string t="";
            int k=i;
            if(st[k]>='0'&&st[k]<='9'){//如果这时候是数字
                while(st[k]>='0'&&st[k]<='9'){//读这些连续的数字
                    t+=st[k];
                    k++;
                }
                i=k-1;//将i进行偏移，因为已经处理到了k-1的位置。
                int flag=t.size()-1;//标记这个时候截取出来的数字的末尾。
                string target="",ans="";
                if(t.size()>8||next_dig){//next_dig是标记是否前面存在小数点，长度>8的，这两种情况都是数字中文直接转换。
                    for(auto l:t){
                        label.push_back(value[l-'0']);//不需要考虑语义，直接数字中文一一对应。
                    }
                    next_dig=false;//处理结束要将小数标记还原。
                }else{
                    while(flag>=0){//在数字还没处理完的时候
                        int p=0;
                        target=t[flag]+target;//每次都是头插，让这个数字保持原样。
                        flag--;//偏移
                        if(target.size()==4||flag==-1){//按照中文语法来处理需要做4位截断，或者说这个数字已经不剩下需要处理的情况。
                            if(flag!=-1){
                                ans=change_char(target,0,p++)+ans;
                            }else{
                                ans=change_char(target,1,p++)+ans;
                            }
                            if(flag!=-1){//如果他这里处理过一次仍未结束，说明他一定是>4位的，那么就需要补上万位。
                                ans="万"+ans;
                            }
                            target="";//把这段待处理的字符串清0
                        }
                    }
                    for(int l=0;l<ans.size();l+=3){
                        label.push_back(ans.substr(l,3));//对ans这段全中文3个字节的截断。
                    }
                }
            }else{
                if((st[i]>='a'&&st[i]<='z')||(st[i]>='A'&&st[i]<='Z')){//对于英文字符，只有碰到非英文字符才会截断，不区分大小写。
                    while(i<st.size()&&((st[i]>='a'&&st[i]<='z')||(st[i]>='A'&&st[i]<='Z'))){
                        t+=st[i];
                        i++;
                    }
                    i--;
                    label.push_back(t); //将英文作为整个单词插入。
                }else{
                    t+=st[i];
                    const string possible_str="零一二三四五六七八九十千万点";
                    if(t=="%"){//如果是百分号，要在数字之前加上百分之
                      int use_count=0;
                      auto iter=label.rbegin();//反向迭代器，查找数字最远在字符串哪里，要在它的前面插入百分之。
                      while(iter!=label.rend()&&possible_str.find(*iter)!= possible_str.npos){//保证当前的这个迭代器值在possible_str或者迭代器不为end
                          use_count++;
                          iter++;
                      }
                      label.insert(label.begin()+label.size()-use_count,"百分之");//可以计算出正向迭代器的位置，直接插入百分之即可。
                      continue;                    
                    }
                    if(t!=","&&t!="!"&&t!="?"&&t!=";"){
                        if(t=="."){//如果是点就翻译成中文的点，然后要记得给next_dig这个标识0的位置要打上标记。
                            t.pop_back();
                            t+="点";
                            next_dig=1;
                        }else{
                          if(t=="+"){//直接翻译，同理
                            t.pop_back();
                            t+="加";
                          }else{
                            if(t=="-"){
                              t.pop_back();
                              t+="减";
                            }
                          }
                        }
                        label.push_back(t);//最后label插入这个t就好，就是一个新的词
                    }
                }
            }
        }
    }
}
int main(int argc,char** argv)
{
    ifstream inFile1(argv[1], ios::in);//读文件，lab文本
    string lineStr;
    while (getline(inFile1, lineStr))//按行读取文件
    {
        if(lineStr[0]==' '){//空行直接丢弃
            continue;
        }
        stringstream ss(lineStr);
        string str;
        vector<string>lineArray;//辅助用的中间字符串数组
        vector<string>label;//标注文本的情况。
        const string a="";
        while (getline(ss, str,' ')){
          if(str!=a){
            lineArray.push_back(str);//如果当前分割出来的不是空字符串，就加入到label数组中。
          }
        }
        if(lineArray.size()==1){
          label.push_back(a);//如果他只有处理文件名称但没有具体文本，那么就以处理文件名称作为键，插入空字符串的值。
        }else{
          for(int i=2;i<lineArray.size();i++){//将有空格分割所有的句子合并到一个句子上待处理。
            lineArray[1]+=' ';
            lineArray[1]+=lineArray[i];
          }
          build_change(lineArray[1],label);//将这个句子转换，并用label存起来，定位至138行。
        }
        for(auto iter=label.begin();iter!=label.end();iter++){//抹除首部的空格。
            if(*iter==" "){
                label.erase(iter);
            }else{
                break;
            }
        }
        label_use[lineArray[0]]=label;//这个处理文件名称作为键，插入label这个标注字符数组。
    }
    inFile1.close();
    return 0;
}
