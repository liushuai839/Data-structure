// 邻接表表示无向图
// 参考：https://github.com/Crystal-Dragon-Liu/C-algorithm_practise/blob/master/C_algorithm/C_algorithm/Dat_Structure/Dat_Structure/Graph_practise.cpp
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <set>
#include <list>
#include <stack>
using namespace std;


// ********************************** 用邻接表 表示无向图*******************************
class Vertex{
public:
    Vertex(int val){ //构造函数
        in_out = 0; //该节点的度数：连接边的数量
        value = val;
    }

    int value;
    int in_out;
    list<Vertex *> neighbours;
};

class undirected_Graph{

public:
    undirected_Graph(string path);  // 构造函数
    int get_num_Vertex();    //获得顶点数量
    int get_num_Edge();      //获得边的数量
    void addEdge(int firstVertex, int secondVertex);   // 输入两个节点添加他们之间的边
    void print_allVertexAndEdge(); 
    map<int, Vertex*> getGraphVertexSet();


private:
    int num_Vertex;  // 无向图中顶点的数量
    int num_Edge;    // 无向图中边的数量    
    map<int, Vertex*> VertexSet;
    bool isInserted(int v,map<int, Vertex*> VertexSet);

};

undirected_Graph::undirected_Graph(string path){
    
    ifstream data(path);
    int dat;
    data >> dat;
    cout << "the count of Vertex : " << dat << " ";
    num_Vertex =dat;
    data >> dat;
    cout << "the count of Edge : " << dat << " ";
    num_Edge = dat;
    cout << endl;

    for(int index = 0; index < num_Edge; index++){
        cout << "读取顶点： " ;
        data >> dat;
        cout << dat << "   ";
        int firstVertex = dat;
        
        cout << "读取顶点： " ;
        data >> dat;
        cout << dat << endl;

        int secondVertex = dat;
        addEdge(firstVertex, secondVertex);
    }
}

void undirected_Graph::addEdge(int firstVertex, int secondVertex){

    Vertex* firstVertexptr = new Vertex(firstVertex);
    Vertex* secondVertexptr = new Vertex(secondVertex);

    firstVertexptr->neighbours.push_back(secondVertexptr);
    firstVertexptr->in_out ++;

    secondVertexptr->neighbours.push_back(firstVertexptr);
    secondVertexptr->in_out ++;

    pair<int, Vertex*> v(firstVertex,firstVertexptr);
    pair<int, Vertex*> w(secondVertex,secondVertexptr);

    //
    if(!isInserted(firstVertex,VertexSet)){
        VertexSet.insert(v);
    }
    else  //如果该节点已经存在的话,则在该节点对应的链表中加入新的值
    {
        VertexSet[firstVertex]->neighbours.push_back(secondVertexptr);
        
    }

    if(!isInserted(secondVertex,VertexSet)){
        VertexSet.insert(w);
    }
    else
    {
        VertexSet[secondVertex]->neighbours.push_back(firstVertexptr);
    }

    
}

// 在顶点集合中 v是否存在
bool undirected_Graph::isInserted(int v, map<int, Vertex*> VertexSet)
{
    map<int, Vertex*>::iterator iter = VertexSet.begin();
    for(; iter!=VertexSet.end(); iter++){
        if(iter->first == v)
            return true;
    }

    return false;
}

// 按照邻接表的形式打印出整个无向图
void undirected_Graph::print_allVertexAndEdge(){
    map<int, Vertex*>:: iterator iter = VertexSet.begin();
    for(; iter != VertexSet.end(); iter++){
        cout << "顶点" << iter->first << "的邻节点： ";
        for(list<Vertex* >::iterator j = iter->second->neighbours.begin();
            j != iter->second->neighbours.end(); j++){
            
            cout << (*j)->value << " ";
        }
        cout << endl;
    }
}

map<int, Vertex*> undirected_Graph::getGraphVertexSet(){
    return VertexSet;
}

int undirected_Graph::get_num_Edge(){
    return num_Edge;
}

int undirected_Graph::get_num_Vertex(){
    return num_Vertex;
}


//************************************************************ 深度优先搜索 **************************************************************************
class DFS{

public: 
    DFS(undirected_Graph uGraph, int s); //构造函数
    bool isMarked(int v);


private: 
    vector<bool> marked;
    void _dfs(undirected_Graph uGraph, int v);

};

DFS::DFS(undirected_Graph uGraph, int s){
    marked = vector<bool>(uGraph.get_num_Edge(),false);
    cout<< "*************** DFS Start****************" << endl;
    _dfs(uGraph,s);
}

void DFS::_dfs(undirected_Graph uGraph, int s){

    cout << "********check " << s << "****************" << endl;
    marked[s] = true;
    
    list<Vertex*> V_uGraphVertexSet = uGraph.getGraphVertexSet()[s]->neighbours;

    list<Vertex*>::iterator iter = V_uGraphVertexSet.begin();
    for(; iter != V_uGraphVertexSet.end(); iter++){
        if( !isMarked((*iter)->value)){
            cout << "************" << (*iter)->value <<" has not been marked ************" << endl;
            _dfs(uGraph,(*iter)->value);
        }
    }

}

bool DFS::isMarked(int w){
    return marked[w];
}


//************************************************************ 深度优先搜索路径 ***********************************************************
class DFS_Search_Path{
public:
    DFS_Search_Path(undirected_Graph uGraph, int s); //构造函数
    bool isMarked(int v);
    void printPaths(); 
    int getLastVertex(int v);     // 获取第一次访问该节点的节点
    int getStartPoint();          // 获取这个类的起始点
    stack<int> pathStore(int v);  // 将路径节点顺序存储(逆序查找，利用stack后进先出的特点，顺序输出)


private:
    vector<bool> marked;
    int startpoint;
    map<int, int> EdgeTo_dfs; //记录每个顶点被第一次访问
    void dfs(undirected_Graph uGraph, int s);
};

bool DFS_Search_Path::isMarked(int w){
    return marked[w];
}

DFS_Search_Path::DFS_Search_Path(undirected_Graph uGraph, int s){
    
    marked = vector<bool>(uGraph.get_num_Edge(),false);
    startpoint = s;
    cout<< "*************** DFS Start****************" << endl;
    dfs(uGraph,s);
}

void DFS_Search_Path::dfs(undirected_Graph uGraph, int s){

    cout << "********check " << s << "****************" << endl;
    marked[s] = true;
    
    list<Vertex*> V_uGraphVertexSet = uGraph.getGraphVertexSet()[s]->neighbours;
    list<Vertex*>::iterator iter = V_uGraphVertexSet.begin();
    for(; iter != V_uGraphVertexSet.end(); iter++){
        if( !isMarked((*iter)->value)){
            cout << "************" << (*iter)->value <<" has not been marked ************" << endl;
            EdgeTo_dfs.insert(pair<int,int>((*iter)->value,s));
            dfs(uGraph,(*iter)->value);
        }
    }
}

void DFS_Search_Path::printPaths(){
    map<int,int>::iterator iter = EdgeTo_dfs.begin();
    cout << "****************print Path**************************" << endl;
    for(; iter != EdgeTo_dfs.end(); iter++){
        cout << (*iter).first << "<----" << (*iter).second << endl;
    }
}

int DFS_Search_Path::getLastVertex(int v){

    auto iter = EdgeTo_dfs.find(v); // 利用查找的方式
    if(iter != EdgeTo_dfs.end()){
        return iter->second;   // 返回第一个访问他的节点
    }
    return -1;
}

int DFS_Search_Path::getStartPoint(){
    return startpoint;
}

stack<int> DFS_Search_Path::pathStore(int v){
    
    stack<int> pathstore; 
    if(!isMarked(v)) // 终点必须是被访问过的
        return stack<int>();
    for(int x = v; x != getStartPoint(); x = getLastVertex(x)){  //! 这个for 循环值得看一下，一般第三个条件都是x++; 
                                                                 //! 其实从本质上看只是为了获取下一次进入循环的x值
                                                                 //! 在这里直接利用函数 获取第一次访问他的节点，并存入路径当中
        pathstore.push(x);
    }

    pathstore.push(getStartPoint());    // 最后加入起点
    return pathstore;
}


//************************************************************ 广度优先搜索 ***********************************************************

class BFS{
public:
    BFS(undirected_Graph uGraph, int s); // 构造函数
    bool isMarked(int w);

private:
    void _bfs(undirected_Graph uGraph, int s);
    vector<bool> marked;
    deque<int> queue_bfs;

};

bool BFS::isMarked(int w){
    return marked[w];
}

BFS::BFS(undirected_Graph uGraph, int s){
    marked = vector<bool>(uGraph.get_num_Edge(),false);
    cout << "******************************* BFS Start **************************************" <<endl;
    _bfs(uGraph, s);
}

void BFS::_bfs(undirected_Graph uGraph, int s){

    marked[s] = true;
    queue_bfs.push_back(s);

    while(queue_bfs.size() != 0){
        int v = queue_bfs.front();
        queue_bfs.pop_front();
        cout << "********check " << v << "****************" << endl;
        list<Vertex*> V_uGraphVertexSet = uGraph.getGraphVertexSet()[v]->neighbours;
        list<Vertex*>::iterator iter = V_uGraphVertexSet.begin();
        for(; iter != V_uGraphVertexSet.end(); iter++){
            if(!isMarked((*iter)->value)){
                cout << "************" << (*iter)->value <<" has not been marked ************" << endl;
                marked[(*iter)->value] = true;
                queue_bfs.push_back((*iter)->value);
            }
        }
    }
}

// **************************************************** 广度优先搜索路径 ****************************************************************

class BFS_Search_Paths{
public:
    BFS_Search_Paths(undirected_Graph uGraph, int s); // 构造函数
    bool isMarked(int w);
    void printPath();
    int getLastVertex(int v);
    int getStartPoint();
    stack<int> pathStore(int v);

private:
    void _bfs(undirected_Graph uGraph, int s);
    int startpoint;
    vector<bool> marked;
    deque<int> queue_bfs;
    map<int,int> EdgeTo_bfs; // 记录第一次访问的节点，和访问该节点的节点
};


bool BFS_Search_Paths::isMarked(int w){
    return marked[w];
}

BFS_Search_Paths::BFS_Search_Paths(undirected_Graph uGraph, int s){
    startpoint = s;
    marked = vector<bool>(uGraph.get_num_Edge(),false);
    cout << "******************************* BFS Start **************************************" <<endl;
    _bfs(uGraph, s);
}

void BFS_Search_Paths::_bfs(undirected_Graph uGraph, int s){

    marked[s] = true;
    queue_bfs.push_back(s);

    while(queue_bfs.size() != 0){
        int v = queue_bfs.front();
        queue_bfs.pop_front();
        cout << "********check " << v << "****************" << endl;
        list<Vertex*> V_uGraphVertexSet = uGraph.getGraphVertexSet()[v]->neighbours;
        list<Vertex*>::iterator iter = V_uGraphVertexSet.begin();
        for(; iter != V_uGraphVertexSet.end(); iter++){
            if(!isMarked((*iter)->value)){
                cout << "************" << (*iter)->value <<" has not been marked ************" << endl;
                EdgeTo_bfs.insert(pair<int,int>((*iter)->value,v));
                marked[(*iter)->value] = true;
                queue_bfs.push_back((*iter)->value);
            }
        }
    }
}

void BFS_Search_Paths::printPath(){
    map<int,int>::iterator iter = EdgeTo_bfs.begin();
    for(; iter != EdgeTo_bfs.end(); iter++){
        cout << (*iter).first << "<---" << (*iter).second <<endl;
    }
}

int BFS_Search_Paths::getLastVertex(int v){

    auto iter = EdgeTo_bfs.find(v); // 利用查找的方式
    if(iter != EdgeTo_bfs.end()){
        return iter->second;   // 返回第一个访问他的节点
    }
    return -1;
}

int BFS_Search_Paths::getStartPoint(){
    return startpoint;
}

stack<int> BFS_Search_Paths::pathStore(int v){
    
    stack<int> pathstore; 
    if(!isMarked(v)) // 终点必须是被访问过的
        return stack<int>();
    for(int x = v; x != getStartPoint(); x = getLastVertex(x)){  //! 这个for 循环值得看一下，一般第三个条件都是x++; 
                                                                 //! 其实从本质上看只是为了获取下一次进入循环的x值
        cout << "向堆栈中存入" << x << endl;                                                      //! 在这里直接利用函数 获取第一次访问他的节点，并存入路径当中
        pathstore.push(x);
    }

    pathstore.push(getStartPoint());    // 最后加入起点
    return pathstore;
}

//************************************************深度优先搜索应用： 查找连通分量 *********************************************************
class connected_components{
private:
    vector<bool> marked;
    map<int,int> id;
    int count = 0;

public:
    connected_components(undirected_Graph uGraph); //构造函数
    void dfs(undirected_Graph uGraph,int v);
    int get_num_components();
    bool isMarked(int w);
    bool isBelongTo(int i,int j);
    void print_Connected_Components();

};

bool connected_components::isMarked(int w){
    return marked[w];
}

void connected_components::dfs(undirected_Graph uGraph,int v){

    marked[v] = true;
    pair<int,int> id_element(v,count); // v是节点的值，count是连通分量的序号 

    id.insert(id_element);

    list<Vertex*> V_uGraphVertexSet = uGraph.getGraphVertexSet()[v]->neighbours;
    // 获取V顶点的邻接表
    list<Vertex*>::iterator iter = V_uGraphVertexSet.begin();
    for(; iter != V_uGraphVertexSet.end(); iter++){
        if(!isMarked((*iter)->value)){
            dfs(uGraph,(*iter)->value);
        }
    }
}

connected_components::connected_components(undirected_Graph uGraph){
    
    marked = vector<bool>(uGraph.get_num_Vertex(),false);
     
    for(int s = 0; s < uGraph.get_num_Vertex(); s++){
        cout << " 这次从" << s << "开始搜索" << endl;
        // 每一次进入if语句都说明是一个新的连通分量
        if(!isMarked(s)){
            dfs(uGraph,s);
            count++;      // count 是连通分量的序号，也可以看成是数量   
        }
    }

    int num_com = get_num_components();

    vector<vector<int> > result(num_com); //! 定义vector要定义容量，不然又报错
    cout << "连通分量的个数：  " << num_com << endl;


    for(int i = 0; i < num_com; i++){
        for(int j = 0; j < uGraph.get_num_Vertex(); j++){

            if(isBelongTo(j,i)){
                result[i].push_back(j);
            }
        }
    }
}

int connected_components::get_num_components(){
    int c = 1;
    int index = id.find(0)->second;
    map<int, int>::iterator iter = id.begin();
    for(iter; iter != id.end(); iter++)
    {
        if((*iter).second != index)
        {
          c++;
            index = (*iter).second;
        }
            
    }
    return c;
}

bool connected_components::isBelongTo(int i, int component){

    if(id.find(i)->second == component)
        return true;
    else
        return false;
}

void connected_components::print_Connected_Components(){
    map<int,int>::iterator iter = id.begin();
    for(; iter != id.end(); iter++){
        cout << (*iter).first << "  " <<(*iter).second << endl; 
    }
}


//***********************************************深度优先搜索应用：判断图中是否有环 *********************************************************
class Cycle{
public:
    Cycle(undirected_Graph Graph);
    bool isMarked(int v);
    void dfs(undirected_Graph uGraph, int s,int v);
    bool hasCycleOrNot();

private:
    vector<bool> marked;
    bool hasCycle = false;

};

bool Cycle::isMarked(int w){
    return marked[w];
}

Cycle::Cycle(undirected_Graph uGraph){
    marked = vector<bool>(uGraph.get_num_Vertex(),false);
    for(int s = 0; s < uGraph.get_num_Vertex(); s++){
        if(!isMarked(s)){
            dfs(uGraph,s,s);
        }
    }
}

void Cycle::dfs(undirected_Graph uGraph, int s,int v){
    marked[s] = true;

    list<Vertex*> V_uGraphVertexSet = uGraph.getGraphVertexSet()[s]->neighbours;
    // 获取V顶点的邻接表
    list<Vertex*>::iterator iter = V_uGraphVertexSet.begin();
    for(; iter != V_uGraphVertexSet.end(); iter++){
        if(!isMarked((*iter)->value)){
            dfs(uGraph,(*iter)->value,s);
        }
        else if((*iter)->value != v){     //! 在深度优先搜索的过程中，如果发现某个节点的相邻节点被标记过了，
                                          //! 而且该相邻节点还不是他的父节点，则证明该图有环
            hasCycle = true; 
        }
    }
}

bool Cycle::hasCycleOrNot(){
    return hasCycle;
}


//***********************************************深度优先搜索应用：判断该图是否为二分图 *********************************************************
class TwoColors{
public:
    TwoColors(undirected_Graph uGraph);
    bool isMarked(int v);
    void dfs(undirected_Graph uGraph,int s);
    bool isBipartite();
private:
    vector<bool> marked;
    vector<bool> color;
    bool isTwocolorable = true;

};

bool TwoColors::isMarked(int w){
    return marked[w];
}

TwoColors::TwoColors(undirected_Graph uGraph){
    marked = vector<bool>(uGraph.get_num_Vertex(),false);
    color = vector<bool>(uGraph.get_num_Vertex(),false);
    for(int s = 0; s < uGraph.get_num_Vertex(); s++){
        if(!isMarked(s))
            dfs(uGraph,s);
    }
}

void TwoColors::dfs(undirected_Graph uGraph,int s){
    marked[s] = true;

    list<Vertex*> V_uGraphVertexSet = uGraph.getGraphVertexSet()[s]->neighbours;
    // 获取V顶点的邻接表
    list<Vertex*>::iterator iter = V_uGraphVertexSet.begin();
    for(; iter != V_uGraphVertexSet.end(); iter++){
        if(!isMarked((*iter)->value)){
            //! 每次向下进行深度优先搜索，都将父节点和子节点的颜色取成相反的颜色
            color[(*iter)->value] = !color[s];
            dfs(uGraph,(*iter)->value);
        }
        //! 一旦遇到被标记的节点，而且父节点和子节点颜色相同，则不是二分图
        else if(color[(*iter)->value] == color[s]){
            isTwocolorable = false;
        }
    }
}

bool TwoColors::isBipartite(){
    return isTwocolorable;
}





int main(){
    string path = "graph_initial.txt";
    undirected_Graph graph(path);
    graph.print_allVertexAndEdge();

    DFS(graph,0);
    cout << "------------------------间隔-------------------------" <<endl;
    int startpoint = 0;
    int endpoint = 4;
    DFS_Search_Path Paths(graph,startpoint);
    Paths.printPaths();

    stack<int> Path_1 = Paths.pathStore(endpoint);
    cout << "DFS_Path" <<endl;
    while(!Path_1.empty()){
        cout << Path_1.top() << endl;
        Path_1.pop();
    } 
    cout << "--" << endl;

    BFS_Search_Paths Path_bfs(graph,0);
    Path_bfs.printPath();
    //! 下面这句话会导致程序卡死，还没有解决
    auto Path_2 = Path_bfs.pathStore(endpoint);
    cout << "BFS_Path" <<endl;
    while(!Path_2.empty()){
        cout << Path_2.top() << endl;
        Path_2.pop();
    } 

    //! 这有问题
    connected_components components(graph);
    components.print_Connected_Components();


    Cycle hascycle_1(graph);
    bool result_1 = hascycle_1.hasCycleOrNot();
    cout << "该图是否有环： " << result_1 << endl;

    TwoColors isTwoColor(graph);
    bool result_2 = isTwoColor.isBipartite();
    cout << "该图是否为二分图： " << result_2 << endl;
    

    return 0;
}