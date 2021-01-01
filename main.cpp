#include <bits/stdc++.h>
#include <time.h>
#define inf INT_MAX

using namespace std;

// implementation of johnson's algorithm using different heaps
// arrays, binary heaps, binomial heaps and fibonacci heaps

// implementation of different data structures
//-a-----------------DS-----------------------//
struct binary_heap
{
    vector<array<int, 2>> pq; // stores the values of distance for each node as {distance, node label}
    vector<int> ref;          // to make find O(1), we store reference to each node in the vector pq

    binary_heap(int n, int s) // n: num of nodes, s: source vertex label
    {
        for (int i = 0; i < n; i++)
        {
            pq.push_back({inf, i});
            ref.push_back(i);
        }
        pq[s][0] = 0;
        moveUp(s);
        moveDown(s);
    }

    void moveUp(int i)
    {
        int p = (i - 1) / 2;
        while (i > 0 && pq[i][0] <= pq[p][0])
        {
            swap(ref[pq[i][1]], ref[pq[p][1]]);
            swap(pq[i][0], pq[p][0]);
            swap(pq[i][1], pq[p][1]);
            i = p;
            p = (i - 1) / 2;
        }
    }

    void moveDown(int i)
    {
        int n = pq.size();
        while (i < n)
        {
            int x = -1;
            if (2 * i + 2 < n)
            {
                if (pq[2 * i + 1][0] < pq[2 * i + 2][0])
                {
                    x = 2 * i + 1;
                }
                else
                {
                    x = 2 * i + 2;
                }
            }
            else if (2 * i + 1 < n)
            {
                x = 2 * i + 1;
            }
            else
                return;
            if (pq[i][0] >= pq[x][0])
            {
                swap(ref[pq[i][1]], ref[pq[x][1]]);
                swap(pq[i][0], pq[x][0]);
                swap(pq[i][1], pq[x][1]);
                i = x;
            }
            else
                return;
        }
    }

    void decreaseKey(int i, int v)
    {
        pq[ref[i]][0] = v;
        moveUp(ref[i]);
    }

    array<int, 2> popMin()
    {
        array<int, 2> x = {pq[0][0], pq[0][1]};
        swap(ref[pq[0][1]], ref[pq.back()[1]]);
        swap(pq[0][0], pq.back()[0]);
        swap(pq[0][1], pq.back()[1]);
        ref[pq.back()[1]] = -1;
        pq.pop_back();
        moveDown(0);
        return x;
    }

    int getValue(int key)
    {
        return pq[ref[key]][0];
    }
};
struct node
{
    int distance, label, degree;
    struct node *parent;
    list<struct node *> children;
};
struct binomial_heap
{
    list<struct node *> heaps;
    vector<struct node *> ref;

    void construct(int n, int s)
    {
        for (int i = 0; i < n; i++)
        {
            struct binomial_heap h;
            struct node *x = new node;
            if (i == s)
                x->distance = 0;
            else
                x->distance = inf;
            x->label = i;
            x->degree = 0;
            x->parent = 0;
            ref.push_back(x);
            h.heaps.push_back(x);
            merge_heap(h);
        }
    }

    void merge_heap(struct binomial_heap &h)
    {
        list<struct node *> heaps_;
        list<struct node *>::iterator it1 = h.heaps.begin(), it2 = heaps.begin();
        while (it1 != h.heaps.end() && it2 != heaps.end())
        {
            if ((*it1)->degree < (*it2)->degree)
            {
                heaps_.push_back(*it1);
                it1++;
            }
            else
            {
                heaps_.push_back(*it2);
                it2++;
            }
        }
        while (it1 != h.heaps.end())
        {
            heaps_.push_back(*it1);
            it1++;
        }
        while (it2 != heaps.end())
        {
            heaps_.push_back(*it2);
            it2++;
        }
        heaps.clear();

        if (heaps_.size() < 2)
        {
            heaps = heaps_;
        }
        else if (heaps_.size() == 2)
        {
            it1 = heaps_.begin();
            it2 = it1;
            it2++;
            if ((*it1)->degree == (*it2)->degree)
            {
                if ((*it1)->distance > (*it2)->distance)
                {
                    swap(it1, it2);
                }
                (*it1)->children.push_back(*it2);
                (*it2)->parent = (*it1);
                (*it1)->degree++;
                heaps.push_back(*it1);
            }
            else
            {
                heaps.push_back(*it1);
                heaps.push_back(*it2);
            }
        }
        else
        {
            it1 = heaps_.begin();
            it2 = it1;
            it2++;
            list<struct node *>::iterator it3 = it2;
            it3++;
            while (it2 != heaps_.end() && it3 != heaps_.end())
            {
                if ((*it1)->degree == (*it2)->degree)
                {
                    if ((*it2)->degree == (*it3)->degree)
                    {
                        if ((*it2)->distance < (*it3)->distance)
                        {
                            swap(*it3, *it2);
                            ref[(*it3)->label] = *it3;
                            ref[(*it2)->label] = *it2;
                        }
                        heaps.push_back(*it1);
                        (*it3)->children.push_back(*it2);
                        (*it2)->parent = (*it3);
                        (*it3)->degree++;
                        it1 = it3;
                        it2 = it1;
                        it2++;
                        it3 = it2;
                        it3++;
                    }
                    else
                    {
                        if ((*it2)->distance > (*it1)->distance)
                        {
                            swap(*it1, *it2);
                            ref[(*it1)->label] = *it1;
                            ref[(*it2)->label] = *it2;
                        }
                        (*it2)->children.push_back(*it1);
                        (*it1)->parent = (*it2);
                        (*it2)->degree++;
                        it1++;
                        it2++;
                        it3++;
                    }
                }
                else
                {
                    heaps.push_back(*it1);
                    it1++;
                    it2++;
                    it3++;
                }
            }
            if (it2 != heaps_.end() && (*it1)->degree == (*it2)->degree)
            {
                if ((*it2)->distance > (*it1)->distance)
                {
                    swap(it1, it2);
                }
                (*it2)->children.push_back(*it1);
                (*it1)->parent = (*it2);
                (*it2)->degree++;
                heaps.push_back(*it2);
            }
            else if (it2 != heaps_.end())
            {
                heaps.push_back(*it1);
                heaps.push_back(*it2);
            }
            else
            {
                heaps.push_back(*it1);
            }
        }
    }

    struct node *popMin()
    {
        struct binomial_heap h;
        struct node *min = *(heaps.begin());
        for (auto it = heaps.begin(); it != heaps.end(); it++)
        {
            if ((*it)->distance < min->distance)
                min = *it;
        }
        struct node *x = min;
        for (auto it = (min->children).begin(); it != (min->children).end(); it++)
        {
            (*it)->parent = 0;
            h.heaps.push_back(*it);
        }
        list<struct node *>::iterator it = heaps.begin(), end = heaps.end();
        while (*(it) != min)
            it++;
        end--;
        list<struct node *>::iterator next = it;
        next++;
        while (it != end)
        {
            (*it) = (*next);
            it++;
            next++;
        }
        heaps.pop_back();
        merge_heap(h);
        (x->children).clear();
        x->degree = 0;
        return x;
    }

    void decreaseKey(int i, int v)
    {
        struct node *x = ref[i];
        struct node *y = x->parent;
        x->distance = v;
        while (y != 0 && x->distance < y->distance)
        {
            swap(x->distance, y->distance);
            swap(ref[x->label], ref[y->label]);
            swap(x->label, y->label);
            x = y;
            y = x->parent;
        }
    }

    int getValue(int i)
    {
        return ref[i]->distance;
    }
};
struct fibnode
{
    int label, distance, degree;
    vector<struct fibnode *> children;
    struct fibnode *parent, *left, *right;
    bool marked;
};
struct fibonacci_heap
{
    struct fibnode *head, *min;
    vector<struct fibnode *> ref;

    fibonacci_heap(int n, int s)
    {
        head = 0;
        for (int i = 0; i < n; i++)
        {
            struct fibnode *x = new (struct fibnode);
            ref.push_back(x);
            x->label = i;
            x->degree = 0;
            x->distance = inf;
            x->parent = x->left = x->right = 0;
            x->marked = 0;
            append(x);
        }
        ref[s]->distance = 0;
        min = ref[s];
    }

    void append(struct fibnode *x)
    {
        x->marked = 0;
        if (head == 0)
        {
            head = x;
            head->left = 0;
            head->right = 0;
        }
        else
        {
            x->left = 0;
            x->right = head;
            head->left = x;
            head = x;
        }
    }

    void remove(struct fibnode *x)
    {
        if (x->left == 0 && x->right == 0)
            head = 0;
        else if (x->left == 0)
        {
            head = x->right;
            (x->right)->left = 0;
            x->right = 0;
        }
        else if (x->right == 0)
        {
            (x->left)->right = 0;
            x->left = 0;
        }
        else
        {
            (x->left)->right = x->right;
            (x->right)->left = x->left;
            x->left = x->right = 0;
        }
    }

    struct fibnode *popMin()
    {
        remove(min);
        for (int i = 0; i < (min->children).size(); i++)
        {
            (min->children)[i]->parent = 0;
            append((min->children)[i]);
        }
        min->degree = 0;
        min->left = min->right = 0;
        (min->children).clear();
        struct fibnode *x = min;
        unordered_map<int, struct fibnode *> rank;
        struct fibnode *it = head;
        min = it;
        while (it)
        {
            if (rank.find(it->degree) == rank.end())
            {
                rank[it->degree] = it;
                if (it->distance < min->distance)
                    min = it;
                it = it->right;
                continue;
            }
            struct fibnode *n1 = rank[it->degree], *n2 = it;
            rank.erase(it->degree);
            if (n2->distance > n1->distance)
            {
                remove(n1);
                n1->right = n2->right;
                n1->left = n2->left;
                if (n1->right)
                    (n1->right)->left = n1;
                if (n1->left)
                    (n1->left)->right = n1;
                n2->left = 0;
                n2->right = 0;

                n2->parent = n1;
                if (n2 == head)
                    head = n1;
                (n1->children).push_back(n2);
                (n1->degree)++;
                it = n1;
                if (it->distance <= min->distance)
                    min = it;
                continue;
            }
            remove(n1);
            n1->parent = n2;
            (n2->children).push_back(n1);
            (n2->degree)++;
            if (n2->distance <= min->distance)
                min = n2;
        }
        return x;
    }

    void del(vector<struct fibnode *> &v, struct fibnode *x)
    {
        int i = 0;
        for (i = 0; i < v.size() - 1; i++)
        {
            if (v[i] == x)
                break;
        }
        for (int j = i; j < v.size() - 1; j++)
        {
            v[j] = v[j + 1];
        }
        v.pop_back();
    }

    void decreaseKey(int i, int v)
    {
        struct fibnode *x = ref[i];
        if (x->parent == 0 || v > (x->parent)->distance)
        {
            x->distance = v;
            if (v < min->distance)
                min = x;
            return;
        }
        struct fibnode *p = x->parent;
        del(p->children, x);
        (p->degree)--;
        x->parent = 0;
        x->distance = v;
        append(x);
        x->marked = 0;
        if (v <= min->distance)
            min = x;
        x = p;
        while (x->marked == 1)
        {
            p = x->parent;
            del(p->children, x);
            (p->degree)--;
            x->parent = 0;
            append(x);
            x->marked = 0;
            x = p;
        }
        if (x->parent)
            x->marked = 1;
    }

    int getValue(int i)
    {
        return (ref[i])->distance;
    }
};
// function for input
void input(int &n, int &d, vector<vector<array<int, 2>>> &adj);

// implementation of johnson's algorithm using various data structures
void array_based();
void binary_heap_based();
void binomial_heap_based();
void fibonacci_heap_based();

// general bellman alogrithm implementation
bool bellman_ford(int &n, vector<vector<array<int, 2>>> &adj, vector<int> &distance);

// helper functions
void modify(int &n, vector<vector<array<int, 2>>> &adj, vector<int> distance);
int get_min(vector<int> &x, vector<bool> &y);

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        fibonacci_heap_based();
        exit(0);
    }
    else if (!strcmp(argv[1], "1"))
        array_based();
    else if (!strcmp(argv[1], "2"))
        binary_heap_based();
    else if (!strcmp(argv[1], "3"))
        binomial_heap_based();
    else
        fibonacci_heap_based();
    exit(0);
}

void input(int &n, int &d, vector<vector<array<int, 2>>> &adj)
{
    cin >> n >> d;
    for (int i = 0; i < n; i++)
    {
        adj.push_back({});
        for (int j = 0; j < n; j++)
        {
            int w;
            cin >> w;
            if (w != 999999 && i != j)
            {
                adj[i].push_back({j, w});
            }
        }
    }
    adj.push_back({});
    for (int j = 0; j < n; j++)
    {
        adj[n].push_back({j, 0});
    }
}

void array_based()
{
    // taking the input and storing graph as adjacency list for each test case
    // for the sake of easier implementation, we take a graph with n+1 nodes instead of n
    // if graph is directed we append the (n+1)th node in the loop itself
    vector<double> time_taken;
    int t;
    cin >> t;
    while (t--)
    {
        int n, d;
        vector<vector<array<int, 2>>> adj;
        input(n, d, adj);
        clock_t time;
        time = clock();

        // program for which time is to be checked starts here
        vector<int> distance(n + 1, inf);
        if (bellman_ford(n, adj, distance))
        {  
            time = clock() - time;
            double time_elapsed = ((double)time) / CLOCKS_PER_SEC;
            time_taken.push_back(time_elapsed);
            cout << -1 << endl;
            continue;
        }
        modify(n, adj, distance);
        // now the adjacency matrix corresponds to the modified graph i.e graph with all positive edges
        // dijkstra over all vertices to obatain all pair shortest paths, these will be stored in 'a'
        vector<vector<int>> a(n, vector<int>(n, inf));

        for (int i = 0; i < n; i++)
        {
            vector<int> x(n, inf); // x will store the distances got so far
            vector<bool> y(n, 0);  // y[i] denotes if edges for i_th node have been relaxed or not
            x[i] = 0;
            for (int it = 0; it < n; it++)
            {
                int mi = get_min(x, y);
                a[i][mi] = x[mi];
                for (int j = 0; j < adj[mi].size(); j++)
                {
                    int u = mi, v = adj[mi][j][0], w = adj[mi][j][1];
                    if (x[u] < inf)
                        x[v] = min(x[v], x[u] + w);
                }
            }
        }
        // program for which time is to be checked ends here
        time = clock() - time;
        double time_elapsed = ((double)time) / CLOCKS_PER_SEC;
        time_taken.push_back(time_elapsed);

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (a[i][j] == inf)
                    cout << 999999 << ' ';
                else
                    cout << a[i][j] + distance[j] - distance[i] << ' ';
            }
            cout << endl;
        }
    }
    for (int i = 0; i < time_taken.size(); i++)
        cout << time_taken[i] << ' ';
    cout << endl;
}

void binary_heap_based()
{
    // taking the input and storing graph as adjacency list for each test case
    // for the sake of easier implementation, we take a graph with n+1 nodes instead of n
    // if graph is directed we append the (n+1)th node in the loop itself
    vector<double> time_taken;
    int t;
    cin >> t;
    while (t--)
    {
        int n, d;
        vector<vector<array<int, 2>>> adj;
        input(n, d, adj);
        clock_t time;
        time = clock();

        // program for which time is to be checked will be written here
        vector<int> distance(n + 1, inf);
        if (bellman_ford(n, adj, distance))
        {  
            time = clock() - time;
            double time_elapsed = ((double)time) / CLOCKS_PER_SEC;
            time_taken.push_back(time_elapsed);
            cout << -1 << endl;
            continue;
        }
        modify(n, adj, distance);
        // now the adjacency matrix corresponds to the modified graph i.e graph with all positive edges
        // dijkstra over all vertices to obatain all pair shortest paths, these will be stored in 'a'
        vector<vector<int>> a(n, vector<int>(n, inf));

        for (int i = 0; i < n; i++)
        {
            vector<bool> x(n, 0);
            binary_heap y(n, i); // this is a binary heap which stores nodes which are yet to be relaxed, initially all nodes except i are set at distance inf, i is at 0
            for (int it = 0; it < n; it++)
            {
                array<int, 2> mm = y.popMin();
                int mi = mm[1], mw = mm[0]; // mi denotes the label of node with least distance out of those present in heap  // mw denotes that distance
                a[i][mi] = mw;
                x[mi] = 1;
                for (int j = 0; j < adj[mi].size(); j++)
                {
                    int u = mi, v = adj[mi][j][0], w = adj[mi][j][1];
                    if (x[v])
                        continue;
                    int w_ = y.getValue(v);
                    if (mw < inf && mw + w < w_)
                    {
                        y.decreaseKey(v, mw + w);
                    }
                }
            }
        }
        // program for which time is to be checked ends here
        time = clock() - time;
        double time_elapsed = ((double)time) / CLOCKS_PER_SEC;
        time_taken.push_back(time_elapsed);

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (a[i][j] == inf)
                    cout << 999999 << ' ';
                else
                    cout << a[i][j] + distance[j] - distance[i] << ' ';
            }
            cout << endl;
        }
    }
    for (int i = 0; i < time_taken.size(); i++)
        cout << time_taken[i] << ' ';
    cout << endl;
}

void binomial_heap_based()
{
    // taking the input and storing graph as adjacency list for each test case
    // for the sake of easier implementation, we take a graph with n+1 nodes instead of n
    // if graph is directed we append the (n+1)th node in the loop itself
    vector<double> time_taken;
    int t;
    cin >> t;
    while (t--)
    {
        int n, d;
        vector<vector<array<int, 2>>> adj;
        input(n, d, adj);
        clock_t time;
        time = clock();

        // program for which time is to be checked will be written here
        vector<int> distance(n + 1, inf);
        if (bellman_ford(n, adj, distance))
        {  
            time = clock() - time;
            double time_elapsed = ((double)time) / CLOCKS_PER_SEC;
            time_taken.push_back(time_elapsed);
            cout << -1 << endl;
            continue;
        }
        modify(n, adj, distance);
        // now the adjacency matrix corresponds to the modified graph i.e graph with all positive edges
        // dijkstra over all vertices to obatain all pair shortest paths, these will be stored in 'a'
        vector<vector<int>> a(n, vector<int>(n, inf));
        for (int i = 0; i < n; i++)
        {
            vector<bool> x(n, 0);
            struct binomial_heap y;
            y.construct(n, i);
            for (int it = 0; it < n; it++)
            {
                struct node *mm = y.popMin();
                int mi = mm->label, mw = mm->distance;
                a[i][mi] = mw;
                x[mi] = 1;
                for (int j = 0; j < adj[mi].size(); j++)
                {
                    int u = mi, v = adj[mi][j][0], w = adj[mi][j][1];
                    if (x[v])
                        continue;
                    int w_ = y.getValue(v);
                    if (mw < inf && mw + w < w_)
                    {
                        y.decreaseKey(v, mw + w);
                    }
                }
            }
        }

        time = clock() - time;
        double time_elapsed = ((double)time) / CLOCKS_PER_SEC;
        time_taken.push_back(time_elapsed);

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (a[i][j] == inf)
                    cout << 999999 << ' ';
                else
                    cout << a[i][j] + distance[j] - distance[i] << ' ';
            }
            cout << endl;
        }
    }
    for (int i = 0; i < time_taken.size(); i++)
        cout << time_taken[i] << ' ';
    cout << endl;
}

void fibonacci_heap_based()
{
    // taking the input and storing graph as adjacency list for each test case
    // for the sake of easier implementation, we take a graph with n+1 nodes instead of n
    // if graph is directed we append the (n+1)th node in the loop itself
    vector<double> time_taken;
    int t;
    cin >> t;
    while (t--)
    {
        int n, d;
        vector<vector<array<int, 2>>> adj;
        input(n, d, adj);
        clock_t time;
        time = clock();

        // program for which time is to be checked will be written here
        vector<int> distance(n + 1, inf);
        if (bellman_ford(n, adj, distance))
        {  
            time = clock() - time;
            double time_elapsed = ((double)time) / CLOCKS_PER_SEC;
            time_taken.push_back(time_elapsed);
            cout << -1 << endl;
            continue;
        }
        modify(n, adj, distance);
        // now the adjacency matrix corresponds to the modified graph i.e graph with all positive edges
        // dijkstra over all vertices to obatain all pair shortest paths, these will be stored in 'a'
        vector<vector<int>> a(n, vector<int>(n, inf));
        for (int i = 0; i < n; i++)
        {
            vector<bool> x(n, 0);
            struct fibonacci_heap y(n, i);
            for (int it = 0; it < n; it++)
            {
                struct fibnode *mm = y.popMin();
                int mi = mm->label, mw = mm->distance;
                a[i][mi] = mw;
                x[mi] = 1;
                for (int j = 0; j < adj[mi].size(); j++)
                {
                    int u = mi, v = adj[mi][j][0], w = adj[mi][j][1];
                    if (x[v])
                        continue;
                    int w_ = y.getValue(v);
                    if (mw < inf && mw + w < w_)
                    {
                        y.decreaseKey(v, mw + w);
                    }
                }
            }
        }

        time = clock() - time;
        double time_elapsed = ((double)time) / CLOCKS_PER_SEC;
        time_taken.push_back(time_elapsed);

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (a[i][j] == inf)
                    cout << 999999 << ' ';
                else
                    cout << a[i][j] + distance[j] - distance[i] << ' ';
            }
            cout << endl;
        }
    }
    for (int i = 0; i < time_taken.size(); i++)
        cout << time_taken[i] << ' ';
    cout << endl;
}

bool bellman_ford(int &n, vector<vector<array<int, 2>>> &adj, vector<int> &distance)
{
    // source is node n, indexing of nodes is done from 0 to n, where n corresponds to the node that is added for johnson's algorithm
    // algorithm is implemented in a simple dp form without any optimizations as specified in the assignment
    // we also detect if any negative cycle is present or not
    bool neg_cycle = 0;
    distance[n] = 0;
    for (int it = 0; it < n + 1; it++)
    {
        for (int i = 0; i < n + 1; i++)
        {
            for (int j = 0; j < adj[i].size(); j++)
            {
                int u = i, v = adj[i][j][0], w = adj[i][j][1];
                if (distance[u] < inf)
                    distance[v] = min(distance[v], distance[u] + w);
            }
        }
    }
    for (int i = 0; i < n + 1; i++)
    {
        for (int j = 0; j < adj[i].size(); j++)
        {
            int u = i, v = adj[i][j][0], w = adj[i][j][1];
            if (distance[v] > distance[u] + w)
                neg_cycle = 1;
        }
    }
    return neg_cycle;
}

void modify(int &n, vector<vector<array<int, 2>>> &adj, vector<int> distance)
{
    adj.pop_back();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < adj[i].size(); j++)
        {
            int u = i, v = adj[i][j][0], w = adj[i][j][1];
            adj[i][j][1] += distance[u] - distance[v];
        }
    }
}

int get_min(vector<int> &x, vector<bool> &y)
{
    int mm = inf, mi = -1;
    for (int i = 0; i < x.size(); i++)
    {
        if (x[i] <= mm && !y[i])
        {
            mi = i;
            mm = x[i];
        }
    }
    y[mi] = 1;
    return mi;
}
