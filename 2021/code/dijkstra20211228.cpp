#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <tuple>
#include <string>
#include <queue>
#include <stack>
#include <thread>
#include <mutex>
#include <chrono>

class Graph {
    int V, E;
    std::map<int, std::vector<std::tuple<int, int, int>>> G;
public:
    bool input_graph(std::string path) {
        std::ifstream file;
        file.open(path, std::ios::in);
        if (!file.is_open())
            return false;
        file >> V;
        file >> E;
        for (int i = 0; i < E; ++i) {
            int u, v, w;
            file >> u >> v >> w;
            G[u].push_back(std::make_tuple(u, v, w));
        }
        return true;
    }
    void show_graph() {
        for (int v = 0; v < V; ++v) {
            std::cout << v << " : ";
            for (auto i = G[v].begin(); i != G[v].end(); ++i)
                std::cout << std::get<0>(*i) << "-(" << std::get<2>(*i) << ")->" << std::get<1>(*i) << "  ";
            std::cout << std::endl;
        }
    }
    int vertices() {
        return V;
    }
    std::vector<std::tuple<int, int, int>> &edges(int v) {
        return G[v];
    }
};

// Dijkstra
class Dijkstra {
protected:
    Graph G;
    int s;
    int *distTo = nullptr;
    int *edgeTo = nullptr;
    std::chrono::time_point<std::chrono::steady_clock> _start, _end;
    std::chrono::duration<double> diff;
    static bool relax(Dijkstra &D, std::tuple<int, int, int> e, int &r) {
        int u = std::get<0>(e);
        int v = std::get<1>(e);
        int w = std::get<2>(e);
        if (D.distTo[v] > D.distTo[u] + w) {
            D.distTo[v] = D.distTo[u] + w;
            D.edgeTo[v] = u;
            r = v;
            return true;
        }
        return false;
    }
    void start() {
        _start = std::chrono::steady_clock::now();
    }
    void end() {
        _end = std::chrono::steady_clock::now();
        diff = _end - _start;
    }
public:
    static const int INFINITY = 99999;
    Dijkstra(Graph &G, int s): G(G), s(s) {
        distTo = new int[G.vertices()];
        edgeTo = new int[G.vertices()];
        _start = std::chrono::steady_clock::now();
        _end = _start;
    }
    ~Dijkstra() {
        delete [] distTo;
        delete [] edgeTo;
    }
    void run() {
        start();
        for (int v = 0; v < G.vertices(); ++v)
            distTo[v] = INFINITY;
        distTo[s] = 0;
        auto dist_greater = [&](const int &i, const int &j) -> bool { return distTo[i] > distTo[j]; };
        std::priority_queue<int, std::vector<int>, decltype(dist_greater)> minHeap(dist_greater);
        minHeap.push(s);
        while (!minHeap.empty()) {
            int v = minHeap.top();
            minHeap.pop();
            auto edges = G.edges(v);
            for (auto i = edges.begin(); i != edges.end(); ++i) {
                int r;
                if (relax(*this, *i, r))
                    minHeap.push(r);
            }
        }
        end();
    }
    void result() {
        std::cout << "Source: " << s << std::endl;
        for (int i = 0; i < G.vertices(); ++i) {
            std::cout << s << "-(" << distTo[i] << ")->" << i << " : ";
            std::stack<int> path;
            for (int j = i; j != s; j = edgeTo[j])
                path.push(j);
            path.push(s);
            while (!path.empty()) {
                std::cout << path.top() << " ";
                path.pop();
            }
            std::cout << std::endl;
        }
    }
    void time() {
        std::cout << "Time: " << diff.count() << std::endl;
    }
};

class MultiDijkstra : Dijkstra {
    int threads;
    std::map<int, std::vector<int>> R;
    static void multi_relax(MultiDijkstra &D, std::vector<std::tuple<int, int, int>> &edges, int id) {
        int e = edges.size();
        for (int i = id; i < e; i += D.threads) {
            int r;
            if (relax(D, edges[i], r))
                D.R[id].push_back(r);
        }
    }
public:
    MultiDijkstra(Graph &G, int s): Dijkstra(G, s) {}
    void run() {
        start();
        for (int v = 0; v < G.vertices(); ++v)
            distTo[v] = INFINITY;
        distTo[s] = 0;
        auto dist_greater = [&](const int &i, const int &j) -> bool { return distTo[i] > distTo[j]; };
        std::priority_queue<int, std::vector<int>, decltype(dist_greater)> minHeap(dist_greater);
        minHeap.push(s);
        while (!minHeap.empty()) {
            int v = minHeap.top();
            minHeap.pop();
            auto edges = G.edges(v);
            threads = 16;
            std::thread t0(multi_relax, std::ref(*this), std::ref(edges), 0);
            std::thread t1(multi_relax, std::ref(*this), std::ref(edges), 1);
            std::thread t2(multi_relax, std::ref(*this), std::ref(edges), 2);
            std::thread t3(multi_relax, std::ref(*this), std::ref(edges), 3);
            std::thread t4(multi_relax, std::ref(*this), std::ref(edges), 4);
            std::thread t5(multi_relax, std::ref(*this), std::ref(edges), 5);
            std::thread t6(multi_relax, std::ref(*this), std::ref(edges), 6);
            std::thread t7(multi_relax, std::ref(*this), std::ref(edges), 7);
            std::thread t8(multi_relax, std::ref(*this), std::ref(edges), 8);
            std::thread t9(multi_relax, std::ref(*this), std::ref(edges), 9);
            std::thread t10(multi_relax, std::ref(*this), std::ref(edges), 10);
            std::thread t11(multi_relax, std::ref(*this), std::ref(edges), 11);
            std::thread t12(multi_relax, std::ref(*this), std::ref(edges), 12);
            std::thread t13(multi_relax, std::ref(*this), std::ref(edges), 13);
            std::thread t14(multi_relax, std::ref(*this), std::ref(edges), 14);
            std::thread t15(multi_relax, std::ref(*this), std::ref(edges), 15);
            t0.join();
            t1.join();
            t2.join();
            t3.join();
            t4.join();
            t5.join();
            t6.join();
            t7.join();
            t8.join();
            t9.join();
            t10.join();
            t11.join();
            t12.join();
            t13.join();
            t14.join();
            t15.join();
            for (int i = 0; i < threads; ++i) {
                auto r = R[i];
                for (auto ii = r.begin(); ii != r.end(); ++ii)
                    minHeap.push(*ii);
                R[i].clear();
            }
        }
        end();
    }
    void result() {
        Dijkstra::result();
    }
    void time() {
        Dijkstra::time();
    }
};

// Main
int main(int argc, char *argv[])
{
    if (argc == 2) {
        Graph G;
        G.input_graph(std::string(argv[1]));
        // G.show_graph();
        // Dijkstra D(G, 0);
        // D.run();
        // D.result();
        // D.time();
        MultiDijkstra MD(G, 0);
        MD.run();
        // MD.result();
        MD.time();
    }
    return 0;
}
