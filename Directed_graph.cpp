// 有向图的基本操作
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



// ********************************** 用邻接表 表示有向图的基本数据机构 *******************************************
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

class Digraph{
public:
    friend class DFS_Search_Path;
    friend class BFS_Search_Paths;
    friend class DirectedCycle;
    friend class DFS_Order;
    friend class Kosaraju;

    Digraph(){};             // 默认构造函数
    Digraph(string path);  // 构造函数
    int get_num_Vertex();    //获得顶点数量
    int get_num_Edge();      //获得边的数量
    void addEdge(int firstVertex, int secondVertex);   // 输入两个节点添加他们之间的边
    void print_allVertexAndEdge(); 
    map<int, Vertex*> getGraphVertexSet();
    Digraph reverse(Digraph graph);  //! 与无向图相比，新添加的函数，用于反转整个有向图


private:
    int num_Vertex;  // 无向图中顶点的数量
    int num_Edge;    // 无向图中边的数量    
    map<int, Vertex*> VertexSet;
    bool isInserted(int v,map<int, Vertex*> VertexSet);

};

Digraph::Digraph(string path){
    
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

void Digraph::addEdge(int firstVertex, int secondVertex){

    Vertex* firstVertexptr = new Vertex(firstVertex);
    Vertex* secondVertexptr = new Vertex(secondVertex);

    //! 与无向图相比，在这只添加一次边的信息
    firstVertexptr->neighbours.push_back(secondVertexptr);
    firstVertexptr->in_out ++;

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
    
}

// 在顶点集合中 v是否存在
bool Digraph::isInserted(int v, map<int, Vertex*> VertexSet)
{
    map<int, Vertex*>::iterator iter = VertexSet.begin();
    for(; iter!=VertexSet.end(); iter++){
        if(iter->first == v)
            return true;
    }

    return false;
}

// 按照邻接表的形式打印出整个无向图
void print_allVertexAndEdge(Digraph graph){
    
    auto VertexSet_1 = graph.getGraphVertexSet();
    map<int, Vertex*>:: iterator iter = VertexSet_1.begin();
    for(; iter != VertexSet_1.end(); iter++){
        cout << "由顶点" << iter->first << "指向的节点： ";
        for(list<Vertex* >::iterator j = iter->second->neighbours.begin();
            j != iter->second->neighbours.end(); j++){
            
            cout << (*j)->value << " ";
        }
        cout << endl;
    }
}

map<int, Vertex*> Digraph::getGraphVertexSet(){
    return VertexSet;
}

int Digraph::get_num_Edge(){
    return num_Edge;
}

int Digraph::get_num_Vertex(){
    return num_Vertex;
}

Digraph Digraph::reverse(Digraph graph){
    
    Digraph graph_reverse ; // 利用默认构造函数
    graph_reverse.num_Edge = graph.num_Edge;
    graph_reverse.num_Vertex = graph.num_Vertex;

    
    map<int, Vertex*>:: iterator iter = VertexSet.begin();
    for(; iter != VertexSet.end(); iter++){
        //iter->first是顶点   (*j)->value是邻接点
        for(list<Vertex* >::iterator j = iter->second->neighbours.begin();
            j != iter->second->neighbours.end(); j++){
            
            graph_reverse.addEdge((*j)->value,iter->first);   
        }
    }

    
    return graph_reverse;
}



//************************************* 深度优先搜索 解决单点连通性的问题 ***************************************************
// TODO 在无向图的基础上改动了一处，用红色标出了
class DFS_Search_Path{
public:
    DFS_Search_Path(Digraph Graph, int s); //构造函数
    bool isMarked(int v);
    void printPaths(); 
    int getLastVertex(int v);     // 获取第一次访问该节点的节点
    int getStartPoint();          // 获取这个类的起始点
    stack<int> pathStore(int v);  // 将路径节点顺序存储(逆序查找，利用stack后进先出的特点，顺序输出)


private:
    vector<bool> marked;
    int startpoint = 0;
    map<int, int> EdgeTo_dfs; //记录每个顶点被第一次访问
    void dfs(Digraph Graph, int s);
};

bool DFS_Search_Path::isMarked(int w){
    return marked[w];
}

DFS_Search_Path::DFS_Search_Path(Digraph Graph, int s){
    
    marked = vector<bool>(Graph.get_num_Edge(),false);
    startpoint = s;
    cout<< "*************** DFS Start****************" << endl;
    dfs(Graph,s);
}

void DFS_Search_Path::dfs(Digraph Graph, int s){

    cout << "********check " << s << "****************" << endl;
    marked[s] = true;
    
    list<Vertex*> V_uGraphVertexSet = Graph.getGraphVertexSet()[s]->neighbours;
    list<Vertex*>::iterator iter = V_uGraphVertexSet.begin();
    for(; iter != V_uGraphVertexSet.end(); iter++){
        if( !isMarked((*iter)->value)){
            cout << "************" << (*iter)->value <<" has not been marked ************" << endl;
            EdgeTo_dfs.insert(pair<int,int>((*iter)->value,s));
            //! 在有向图中新加的，区别与无向图，由于连通的路径是单向的，
            //! 所以在 VertexSet(包含所有顶点的map) 中不到那些没有 出度 为0的节点,就会导致map越界的错误
            if(Graph.VertexSet.find((*iter)->value) != Graph.VertexSet.end())
                dfs(Graph,(*iter)->value);
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

//***************************** 广度优先搜索，解决单点最短路径的问题 ********************************************
// TODO 在无向图的基础上改动了一处，用红色标出了(同深度优先搜索)
class BFS_Search_Paths{
public:
    BFS_Search_Paths(Digraph Graphh, int s); // 构造函数
    bool isMarked(int w);
    void printPath();
    int getLastVertex(int v);
    int getStartPoint();
    stack<int> pathStore(int v);

private:
    void _bfs(Digraph Graphh, int s);
    int startpoint;
    vector<bool> marked;
    deque<int> queue_bfs;
    map<int,int> EdgeTo_bfs; // 记录第一次访问的节点，和访问该节点的节点
};


bool BFS_Search_Paths::isMarked(int w){
    return marked[w];
}

BFS_Search_Paths::BFS_Search_Paths(Digraph Graphh, int s){
    startpoint = s;
    marked = vector<bool>(Graphh.get_num_Edge(),false);
    cout << "******************************* BFS Start **************************************" <<endl;
    _bfs(Graphh, s);
}

void BFS_Search_Paths::_bfs(Digraph Graph, int s){

    marked[s] = true;
    queue_bfs.push_back(s);

    while(queue_bfs.size() != 0){
        int v = queue_bfs.front();
        queue_bfs.pop_front();
        cout << "********check " << v << "****************" << endl;
        //! 和 深度优先搜索一样，在 VertexSet(包含所有顶点的map) 中不到那些没有 出度 为0的节点,
        //! 就会导致map越界的错误，所以直接跳过
        if(Graph.VertexSet.find(v) == Graph.VertexSet.end())
        {
            //cout << "节点 " << v << "不存在子节点,直接跳过！！" << endl;
            continue;
        }
        
        list<Vertex*> V_uGraphVertexSet = Graph.getGraphVertexSet()[v]->neighbours;
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
    for(int x = v; x != getStartPoint() ; x = getLastVertex(x)){  //! 这个for 循环值得看一下，一般第三个条件都是x++; 
                                                                 //! 其实从本质上看只是为了获取下一次进入循环的x值
                                                                 //! 在这里直接利用函数 获取第一次访问他的节点，并存入路径当中
        pathstore.push(x);
    }

    pathstore.push(getStartPoint());    // 最后加入起点
    return pathstore;
}

// *********************************** 在有向图中检测是否有环 ************************************************
class DirectedCycle{
public:
    DirectedCycle(Digraph graph); // 构造函数
    bool isMarked(int w);
    void dfs(Digraph graph,int w);
    bool hascycle();

private:
    vector<bool> marked;
    vector<bool> onStack;
    stack<int> Cycle;
    map<int,int> EdgeTo;

};

DirectedCycle::DirectedCycle(Digraph graph){
    marked = vector<bool>(graph.get_num_Vertex(),false);
    onStack = vector<bool>(graph.get_num_Vertex(),false);
    for(int i = 0; i < graph.get_num_Vertex(); i++){
        if(graph.VertexSet.find(i) != graph.VertexSet.end()  &&
           !isMarked(i)){
               dfs(graph,i);
           }
    }
}

bool DirectedCycle::isMarked(int w){
    return marked[w];
}

//! 有向图检测是否有环原理： 在一个连通区域之中，会将onStack相应的位置设为true,
//! 然后在这个连通区域内如果遇到环(之前搜索过的节点，相应位置已经设为true),则进入第三个if语句，将重复值存进Cycle中
//! 最后根据Cycle是否为空，判断该有向图是否有环
//! 即Cycle 不为空，则有环；为空，则无环
void DirectedCycle::dfs(Digraph graph,int w){
    marked[w] = true;
    onStack[w] = true;
    list<Vertex*> V_uGraphVertexSet = graph.getGraphVertexSet()[w]->neighbours;
    list<Vertex*>::iterator iter = V_uGraphVertexSet.begin();
    for(; iter != V_uGraphVertexSet.end(); iter++){
        if(hascycle())
            return;
        
        else if( !isMarked((*iter)->value)){
            //cout << "************" << (*iter)->value <<" has not been marked ************" << endl;
            EdgeTo.insert(pair<int,int>((*iter)->value,w));
            //! 在有向图中新加的，区别与无向图，由于连通的路径是单向的，
            //! 所以在 VertexSet(包含所有顶点的map) 中不到那些没有 出度 为0的节点,就会导致map越界的错误
            if(graph.VertexSet.find((*iter)->value) != graph.VertexSet.end())
                dfs(graph,(*iter)->value);
        }

        else if(onStack[(*iter)->value]){
            for(int x = w; x != (*iter)->value; x = EdgeTo[x]){
                Cycle.push(x);
            }
                
            Cycle.push((*iter)->value);
            Cycle.push(w);
        }
    }
    onStack[w] = false;
}

bool DirectedCycle::hascycle(){
    return !Cycle.empty() ;
}

// *******************************  有向图中基于深度优先搜索的顶点排序 ******************************************
class DFS_Order{
public:
    friend class Kosaraju;
    vector<bool> marked; 
    DFS_Order(Digraph graph);
    bool isMarked(int w);
    void dfs(Digraph graph,int w);
    void print_pre();
    void print_post();
    void print_reverse_post();
private:
    deque<int> pre;  // 所有顶点的先序排列
    deque<int> post; // 所有顶点的后序排列
    stack<int> reverse_post;  // 所有顶点的逆后序排列
};
DFS_Order::DFS_Order(Digraph graph){

    marked = vector<bool>(graph.get_num_Vertex(),false);
    for(int i = 0; i < graph.get_num_Vertex(); i++){
        if(!isMarked(i)){
            dfs(graph,i);
        }
    }
}

bool DFS_Order::isMarked(int w){
    return marked[w];
}

void DFS_Order::dfs(Digraph graph,int w){
    marked[w] = true;
    pre.push_back(w);

    // 如果某些顶点不在顶点顶点集合当中，则不进行下一步的深度优先搜索
    if(graph.VertexSet.find(w) == graph.VertexSet.end()){
        //cout << w << "不在顶点集合当中" << endl;
        post.push_back(w);
        reverse_post.push(w);
        return ;
    }

    list<Vertex*> V_uGraphVertexSet = graph.getGraphVertexSet()[w]->neighbours;
    list<Vertex*>::iterator iter = V_uGraphVertexSet.begin();
    for(; iter != V_uGraphVertexSet.end(); iter++){
        if( !isMarked((*iter)->value)){
            //! 在有向图中新加的，区别与无向图，由于连通的路径是单向的，
            //! 所以在 VertexSet(包含所有顶点的map) 中不到那些没有 出度 为0的节点,就会导致map越界的错误
                dfs(graph,(*iter)->value);
        }
    }
    post.push_back(w);
    reverse_post.push(w);
}

void DFS_Order::print_pre(){
    for(int i = 0; i < pre.size(); i++){
        cout << pre[i] <<endl;
    }
}

void DFS_Order::print_post(){
    for(int i = 0; i < post.size(); i++){
        cout << post[i] <<endl;
    }
}

void DFS_Order::print_reverse_post(){
    while(!reverse_post.empty()){
        cout << reverse_post.top() <<endl;
        reverse_post.pop();
    }
}

// ************************************有向图中的强连通分量计算(Kosaraju 算法) ****************************************************

class Kosaraju{
public:
    vector<bool> marked;
    Kosaraju(Digraph gragh); // 构造函数
    bool isMarked(int w);
    void dfs(Digraph gragh, int w);
    int get_num_components();
    bool isBelongTo(int i,int j);
    void print_Connected_Components();

private:
    int count = 0; // 强连通分量数量
    map<int,int> id;

};

Kosaraju::Kosaraju(Digraph gragh){
    marked = vector<bool>(gragh.get_num_Vertex(),false);
    DFS_Order order(gragh.reverse(gragh));
    auto re_post_stack =  order.reverse_post;
    vector<int> re_post_vec(0);


    // 将逆后序的stack 用vector存储
    while(!re_post_stack.empty()){
        int temp = re_post_stack.top();
        re_post_vec.push_back(temp);
        re_post_stack.pop();
    }
    
    // for(int i = 0; i < re_post_vec.size(); i++){
    //     cout << "逆图的逆后序排列 " << endl;
    //     cout << re_post_vec[i] << endl;
    // }


    
    for(int i = 0; i < re_post_vec.size(); i++){
        if( gragh.VertexSet.find(re_post_vec[i]) != gragh.VertexSet.end()  &&
            !isMarked(re_post_vec[i])){
            dfs(gragh,re_post_vec[i]);
            count++;
        }
    }
    
    int num_com = get_num_components();
    vector<vector<int> > result(num_com); //! 定义vector要定义容量，不然又报错
    cout << "连通分量的个数：  " << num_com << endl;


    for(int i = 0; i < num_com; i++){
        for(int j = 0; j < gragh.get_num_Edge(); j++){

            if(isBelongTo(j,i)){
                result[i].push_back(j);
            }
        }
    }


}

bool Kosaraju::isMarked(int w){
    return marked[w];
}

void Kosaraju::dfs(Digraph Graph,int v){
    //cout << "此时dfs搜索： " << v << endl;
    marked[v] = true;
    pair<int,int> id_element(v,count); // v是节点的值，count是连通分量的序号 
    id.insert(id_element);

    if(Graph.VertexSet.find(v) == Graph.VertexSet.end()){
        //cout << v <<" 不在顶点集合当中" << endl;
        return;
    }

    list<Vertex*> V_uGraphVertexSet = Graph.getGraphVertexSet()[v]->neighbours;
    // 获取V顶点的邻接表
    list<Vertex*>::iterator iter = V_uGraphVertexSet.begin();
    for(; iter != V_uGraphVertexSet.end(); iter++){
        if(!isMarked((*iter)->value)){
            dfs(Graph,(*iter)->value);
        }
    }
}

int Kosaraju::get_num_components(){
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

bool Kosaraju::isBelongTo(int i, int component){

    if(id.find(i)->second == component)
        return true;
    else
        return false;
}

void Kosaraju::print_Connected_Components(){
    map<int,int>::iterator iter = id.begin();
    
    for(; iter != id.end(); iter++){
        cout << (*iter).first << "  " <<(*iter).second << endl; 
    }
}





int main(){
                
    string path = "graph_initial_1.txt";
    Digraph graph(path);
    print_allVertexAndEdge(graph);
    cout << "*****************   逆图  ******************************" << endl;
    Digraph graph_reverse = graph.reverse(graph);
    print_allVertexAndEdge(graph_reverse);



// //******************************** DFS-Search-Path **************************
    cout << "******************************** DFS-Search-Path **************************" << endl;
    int startpoint = 0;
    int endpoint = 2;
    DFS_Search_Path Paths(graph,startpoint);
    Paths.printPaths();

    stack<int> Path_1 = Paths.pathStore(endpoint);
    cout << "DFS_Path" <<endl;
    while(!Path_1.empty()){
        cout << Path_1.top() << endl;
        Path_1.pop();
    } 
// //******************************** BFS-Search-Path **************************
    cout << "******************************** BFS-Search-Path **************************" << endl;
    BFS_Search_Paths Path_bfs(graph,0);
    Path_bfs.printPath();

    auto Path_2 = Path_bfs.pathStore(endpoint);
    cout << "BFS_Path" <<endl;
    while(!Path_2.empty()){
        cout << Path_2.top() << endl;
        Path_2.pop();
    } 
// ****************************** 检测有向图是否有环 ****************************
    cout << "****************************** 检测有向图是否有环 ****************************" << endl;
    DirectedCycle hasCycle(graph);
    bool result_1 = hasCycle.hascycle();
    cout << "该有向图是否有环： " << result_1 << endl;
// **********************  有向图中基于深度优先搜索的顶点排序 ********************************************
    cout << "**********************  有向图中基于深度优先搜索的顶点排序 ********************************************" << endl;
    DFS_Order order(graph);
    cout << "有向图顶点先序遍历结果" << endl;
    order.print_pre();
    cout << "有向图顶点后序遍历结果" << endl;
    order.print_post();
    cout << "有向图顶点逆后序遍历结果" << endl;
    order.print_reverse_post();
// *********************** 有向图的强连通分量*********************************************
    // 求强连通分量的时候还有点bug，比如graph_initial_1.txt,答案是5，程序跑出来是4，节点1其实应该单独算一个强连通分量
    
    cout << "*********************** 有向图的强连通分量*********************************************" << endl;
    Kosaraju components(graph);
    components.print_Connected_Components();

}
