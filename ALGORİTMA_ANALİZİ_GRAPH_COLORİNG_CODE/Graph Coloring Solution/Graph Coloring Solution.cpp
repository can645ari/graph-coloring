#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <clocale>
#include <time.h>

using namespace std;

class Graph {
private:
    int numNodes;
    unordered_map<int, vector<int>> adjacencyList;

public:
    Graph(int numNodes) : numNodes(numNodes) {}

    void addEdge(int x, int y) {
        adjacencyList[x].push_back(y);
        adjacencyList[y].push_back(x); // Eğer yönlendirilmemiş bir graf oluşturuyorsanız bu satırı kaldırabilirsiniz
    }

    void printGraph() {
        for (const auto& pair : adjacencyList) {
            cout << pair.first << ": ";
            for (int neighbor : pair.second) {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }

    void grafColoring() {
        vector<int> colors(numNodes, -1); // Her düğüm için renklerin dizisi, -1: henüz renklendirilmemiş
        unordered_set<int> availableColors; // Mevcut boş renklerin kümesi

        // Başlangıçta tüm renkler mevcuttur
        for (int i = 0; i < numNodes; ++i) {
            availableColors.insert(i);
        }

        // Her düğümü ziyaret ederek renklendirme
        for (int node = 0; node < numNodes; ++node) {
            // Düğümün komşularını kontrol et
            for (int neighbor : adjacencyList[node]) {
                if (colors[neighbor] != -1) { // Komşunun rengi atanmışsa, mevcut renklerden çıkar
                    availableColors.erase(colors[neighbor]);
                }
            }
            // Mevcut renklerden birini düğüme ata
            colors[node] = *availableColors.begin();
            // Mevcut renkleri sıfırla
            availableColors.clear();
            // Tüm renkleri tekrar mevcut hale getir
            for (int i = 0; i < numNodes; ++i) {
                availableColors.insert(i);
            }
        }

        // Sonuçları yazdır
        for (int i = 0; i < numNodes; ++i) {
            cout << "köşe " << i << " --> renk " << colors[i] << endl;
        }
    }

    void welshPowell(const Graph& graph) {
        // Grafı düğüm derecelerine göre sırala
        vector<pair<int, int>> nodeDegrees;
        for (const auto& entry : graph.adjacencyList) {
            nodeDegrees.push_back({ entry.first, entry.second.size() });
        }
        sort(nodeDegrees.begin(), nodeDegrees.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
            return a.second > b.second;
            });

        // Her düğüm için kullanılan renkleri saklayacak map
        unordered_map<int, int> colors;

        // Kullanılan renk sayısı
        int numColors = 0;

        // Her düğümü sırayla işle
        for (const auto& nodeDegree : nodeDegrees) {
            int node = nodeDegree.first;
            // Kullanılan renklerin kümesi
            unordered_set<int> usedColors;
            for (int neighbour : graph.adjacencyList.at(node)) {
                usedColors.insert(colors[neighbour]);
            }

            // Kullanılabilecek en küçük renk
            int color = -1;
            for (int c = 0; c < numColors; ++c) {
                if (usedColors.find(c) == usedColors.end()) {
                    color = c;
                    break;
                }
            }

            // Yeni bir renk kullanılacaksa, kullanılan renk sayısını artır
            if (color == -1) {
                color = numColors;
                ++numColors;
            }

            // Düğüme rengi ata
            colors[node] = color;
        }

        // Sonuçları yazdır
        cout << "Kullanılan toplam renk sayısı: " << numColors << endl;
        for (int i = 0; i < numNodes; ++i) {
            cout << "köşe " << i << " --> renk " << colors[i] << endl;
        }
    }
};

int main() {
    clock_t tStart = clock();
    setlocale(LC_ALL, "Turkish");
    /*
    gc_20_1" / gc_50_7" / gc_70_1" / gc_500_3" / gc_1000_9"
    */
    ifstream inputFile("..\\data\\gc_1000_9");
    if (!inputFile.is_open()) {
        cerr << "Dosya açılamadı!" << endl;
        return 1;
    }

    int numNodes, numEdges;
    inputFile >> numNodes >> numEdges;

    Graph graph(numNodes);

    for (int i = 0; i < numEdges; ++i) {
        int x, y;
        inputFile >> x >> y;
        graph.addEdge(x, y);
    }

    inputFile.close();

    graph.welshPowell(graph);
    printf("Çalışma süresi: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

    return 0;
}
