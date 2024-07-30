#include "BPlusTree.h"

Node* parent = NULL;

//Node::ptr::ptr() {}

Node::Node() {
    this->isLeaf = false;
    this->ptr2next = NULL;
}

BPlusTree::BPlusTree() {
    this->maxIntChildLimit = 4;
    this->maxLeafNodeLimit = 3;
    this->root = NULL;
}

BPlusTree::BPlusTree(int degreeInternal, int degreeLeaf) {
    this->maxIntChildLimit = degreeInternal;
    this->maxLeafNodeLimit = degreeLeaf;
    this->root = NULL;
}

int BPlusTree::getMaxIntChildLimit() {
    return maxIntChildLimit;
}

int BPlusTree::getMaxLeafNodeLimit() {
    return maxLeafNodeLimit;
}

Node* BPlusTree::getRoot() {
    return this->root;
}

void BPlusTree::setRoot(Node* ptr) {
    this->root = ptr;
}

Node* BPlusTree::firstLeftNode(Node* cursor) {
    if (cursor->isLeaf) {
        return cursor;
    }
    for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
        if (cursor->ptr2TreeOrData.ptr2Tree[i] != NULL) {
            return firstLeftNode(cursor->ptr2TreeOrData.ptr2Tree[i]);
        }
    }
    return NULL;
}

Node** BPlusTree::findParent(Node* cursor, Node* child) {
    if (cursor->isLeaf || cursor->ptr2TreeOrData.ptr2Tree[0]->isLeaf) {
        return NULL;
    }
    for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
        if (cursor->ptr2TreeOrData.ptr2Tree[i] == child) {
            parent = cursor;
        }
        else {
            Node* tempCursor = cursor->ptr2TreeOrData.ptr2Tree[i];
            findParent(tempCursor, child);
        }
    }
    return &parent;
}

void BPlusTree::display(Node* cursor) {
    if (cursor != NULL) {
        for (int i = 0; i < cursor->keys.size(); i++) {
            cout << cursor->keys[i] << " ";
        }
        cout << endl;
        if (!cursor->isLeaf) {
            for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
                display(cursor->ptr2TreeOrData.ptr2Tree[i]);
            }
        }
    }
}

void BPlusTree::seqDisplay(Node* cursor) {
    Node* firstLeft = firstLeftNode(cursor);

    if (firstLeft == NULL) {
        cout << "No hay datos." << endl;
        return;
    }

    while (firstLeft != NULL) {
        for (int i = 0; i < firstLeft->keys.size(); i++) {
            cout << firstLeft->keys[i] << " ";
        }
        firstLeft = firstLeft->ptr2next;
    }
    cout << endl;
}

string BPlusTree::search(int key) {
    if (root == NULL) {
        cout << "Tree is empty\n";
        return "";
    } else {
        Node* cursor = root;
        while (cursor != NULL) {
            for (int i = 0; i < cursor->keys.size(); i++) {
                if (key < cursor->keys[i]) {
                    cursor = cursor->ptr2TreeOrData.ptr2Tree[i];
                    break;
                }
                if (i == cursor->keys.size() - 1) {
                    cursor = cursor->ptr2TreeOrData.ptr2Tree[i + 1];
                    break;
                }
            }
            if (cursor->isLeaf) {
                for (int i = 0; i < cursor->keys.size(); i++) {
                    if (cursor->keys[i] == key) {
                        cout << "Found\n";
                        string fileName = "../Archivos/";
                        fileName += to_string(key) + ".txt";
                        ifstream file(fileName);

                        if (file.is_open()) {
                            cout << "Clave Encontrada." << endl;
                            cout << "Data correspondiente es: ";

                            vector<string> lines;
                            string line;
                            while (getline(file, line)) {
                                lines.push_back(line);
                            }
                            file.close();
                            for (size_t i = 0; i < lines.size(); ++i) {
                                cout << i + 1 << ": " << lines[i] << endl;
                            }

                            if (lines.size() > 1) {
                                int lineNumber;
                                cout << "El archivo tiene más de una línea. Ingrese el número de la línea que desea analizar: ";
                                cin >> lineNumber;

                                while (lineNumber < 1 || lineNumber > lines.size()) {
                                    cout << "Número de línea no válido. Por favor, ingrese un número entre 1 y " << lines.size() << ": ";
                                    cin >> lineNumber;
                                }

                                line = lines[lineNumber - 1];
                            } else {
                                line = lines[0];
                            }

                            cout << "Línea seleccionada: " << line << endl;
                            return line;
                        } else {
                            cout << "No se pudo abrir el archivo." << endl;
                            return "";
                        }

                    }
                }
                cout << "Not found\n";
                return "";
            }
        }
        cout << "Not found\n";
        return "";
    }
}

void BPlusTree::insert(int key, FILE* filePtr, const string& additionalData) {
    if (root == NULL) {
        root = new Node;
        root->isLeaf = true;
        root->keys.push_back(key);
        new (&root->ptr2TreeOrData.dataPtr) vector<FILE*>;
        root->ptr2TreeOrData.dataPtr.push_back(filePtr);
        cout << key << ": es la Raiz" << endl;
        return;
    }

    Node* cursor = root;
    Node* parent = NULL;
    while (!cursor->isLeaf) {
        parent = cursor;
        int index = upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
        cursor = cursor->ptr2TreeOrData.ptr2Tree[index];
    }
    for (int i = 0; i < cursor->keys.size(); i++) {
        if (cursor->keys[i] == key) {
            cout << "La clave ya existe. ¿Desea abrir el archivo existente para agregar más datos? (s/n): ";
            char response;
            cin >> response;
            if (response == 's' || response == 'S') {
                string fileName = "../Archivos/" + to_string(key) + ".txt";
                ofstream existingFile(fileName, ios::app);
                if (existingFile.is_open()) {
                    existingFile << key << " " << additionalData << "\n";
                    existingFile.close();
                    cout << "Datos adicionales agregados al archivo existente." << endl;
                } else {
                    cout << "No se pudo abrir el archivo." << endl;
                }
            }
            return;
        }
    }

    if (cursor->keys.size() < maxLeafNodeLimit) {
        int i = upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
        cursor->keys.insert(cursor->keys.begin() + i, key);
        cursor->ptr2TreeOrData.dataPtr.insert(cursor->ptr2TreeOrData.dataPtr.begin() + i, filePtr);
        cout << "Insercion completada: " << key << endl;
    } else {
        vector<int> virtualNode(cursor->keys);
        vector<FILE*> virtualDataNode(cursor->ptr2TreeOrData.dataPtr);

        int i = upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
        virtualNode.insert(virtualNode.begin() + i, key);
        virtualDataNode.insert(virtualDataNode.begin() + i, filePtr);

        Node* newLeaf = new Node;
        newLeaf->isLeaf = true;
        new (&newLeaf->ptr2TreeOrData.dataPtr) vector<FILE*>;

        Node* temp = cursor->ptr2next;
        cursor->ptr2next = newLeaf;
        newLeaf->ptr2next = temp;

        cursor->keys.assign(virtualNode.begin(), virtualNode.begin() + maxLeafNodeLimit / 2);
        cursor->ptr2TreeOrData.dataPtr.assign(virtualDataNode.begin(), virtualDataNode.begin() + maxLeafNodeLimit / 2);

        newLeaf->keys.assign(virtualNode.begin() + maxLeafNodeLimit / 2, virtualNode.end());
        newLeaf->ptr2TreeOrData.dataPtr.assign(virtualDataNode.begin() + maxLeafNodeLimit / 2, virtualDataNode.end());

        if (cursor == root) {
            Node* newRoot = new Node;
            newRoot->keys.push_back(newLeaf->keys[0]);
            new (&newRoot->ptr2TreeOrData.ptr2Tree) vector<Node*>;
            newRoot->ptr2TreeOrData.ptr2Tree.push_back(cursor);
            newRoot->ptr2TreeOrData.ptr2Tree.push_back(newLeaf);
            root = newRoot;
            cout << "Creando nueva Raiz." << endl;
        } else {
            insertInternal(newLeaf->keys[0], parent, newLeaf);
        }
    }
}

void BPlusTree::insertInternal(int x, Node* cursor, Node* child) {
    if (cursor->keys.size() < maxIntChildLimit - 1) {
        int i = upper_bound(cursor->keys.begin(), cursor->keys.end(), x) - cursor->keys.begin();
        cursor->keys.insert(cursor->keys.begin() + i, x);
        cursor->ptr2TreeOrData.ptr2Tree.insert(cursor->ptr2TreeOrData.ptr2Tree.begin() + i + 1, child);
        cout << "Clave insertada en el nodo interno." << endl;
    } else {
        vector<int> virtualKeyNode(cursor->keys);
        vector<Node*> virtualTreePtrNode(cursor->ptr2TreeOrData.ptr2Tree);

        int i = upper_bound(virtualKeyNode.begin(), virtualKeyNode.end(), x) - virtualKeyNode.begin();
        virtualKeyNode.insert(virtualKeyNode.begin() + i, x);
        virtualTreePtrNode.insert(virtualTreePtrNode.begin() + i + 1, child);

        int partitionKey = virtualKeyNode[virtualKeyNode.size() / 2];
        int partitionIndex = virtualKeyNode.size() / 2;

        cursor->keys.assign(virtualKeyNode.begin(), virtualKeyNode.begin() + partitionIndex);
        cursor->ptr2TreeOrData.ptr2Tree.assign(virtualTreePtrNode.begin(), virtualTreePtrNode.begin() + partitionIndex + 1);

        Node* newInternalNode = new Node;
        new (&newInternalNode->ptr2TreeOrData.ptr2Tree) vector<Node*>;
        newInternalNode->keys.assign(virtualKeyNode.begin() + partitionIndex + 1, virtualKeyNode.end());
        newInternalNode->ptr2TreeOrData.ptr2Tree.assign(virtualTreePtrNode.begin() + partitionIndex + 1, virtualTreePtrNode.end());

        if (cursor == root) {
            Node* newRoot = new Node;
            newRoot->keys.push_back(partitionKey);
            new (&newRoot->ptr2TreeOrData.ptr2Tree) vector<Node*>;
            newRoot->ptr2TreeOrData.ptr2Tree.push_back(cursor);
            newRoot->ptr2TreeOrData.ptr2Tree.push_back(newInternalNode);
            root = newRoot;
            cout << "Raiz nueva creada." << endl;
        } else {
            Node** parentPtr = findParent(root, cursor);
            insertInternal(partitionKey, *parentPtr, newInternalNode);
        }
    }
}

void BPlusTree::removeKey(int x) {
    Node* root = getRoot();

    if (root == NULL) {
        cout << "El Árbol B+ está vacío" << endl;
        return;
    }

    Node* cursor = root;
    Node* parent = NULL;
    int leftSibling, rightSibling;

    while (!cursor->isLeaf) {
        parent = cursor;
        for (int i = 0; i < cursor->keys.size(); i++) {
            if (x < cursor->keys[i]) {
                leftSibling = i - 1;
                rightSibling = i + 1;
                cursor = cursor->ptr2TreeOrData.ptr2Tree[i];
                break;
            }
            if (i == cursor->keys.size() - 1) {
                leftSibling = i;
                rightSibling = i + 1;
                cursor = cursor->ptr2TreeOrData.ptr2Tree[i + 1];
                break;
            }
        }
    }

    int pos = 0;
    bool found = false;
    for (pos = 0; pos < cursor->keys.size(); pos++) {
        if (cursor->keys[pos] == x) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Clave no encontrada en el árbol" << endl;
        return;
    }

    string fileName = "../Archivos/" + to_string(x) + ".txt";
    if (remove(fileName.c_str()) != 0) {
        perror("Error al eliminar el archivo");
    } else {
        cout << "Archivo eliminado exitosamente" << endl;
    }

    cursor->keys.erase(cursor->keys.begin() + pos);

    if (cursor == root) {
        if (cursor->keys.size() == 0) {
            setRoot(NULL);
            cout << "¡Ohh! ¡Nuestro árbol está vacío ahora! :(" << endl;
        }
        return;
    }

    if (cursor->keys.size() >= (maxLeafNodeLimit + 1) / 2) {
        return;
    }

    if (leftSibling >= 0) {
        Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];

        if (leftNode->keys.size() >= (maxLeafNodeLimit + 1) / 2 + 1) {
            cursor->keys.insert(cursor->keys.begin(), leftNode->keys.back());
            leftNode->keys.pop_back();
            parent->keys[leftSibling] = cursor->keys[0];
            return;
        }
    }

    if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
        Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];

        if (rightNode->keys.size() >= (maxLeafNodeLimit + 1) / 2 + 1) {
            cursor->keys.push_back(rightNode->keys.front());
            rightNode->keys.erase(rightNode->keys.begin());
            parent->keys[rightSibling - 1] = rightNode->keys[0];
            return;
        }
    }

    if (leftSibling >= 0) {
        Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];
        leftNode->keys.insert(leftNode->keys.end(), cursor->keys.begin(), cursor->keys.end());
        leftNode->ptr2next = cursor->ptr2next;
        removeInternal(parent->keys[leftSibling], parent, cursor);
        delete cursor;
    } else if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
        Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];
        cursor->keys.insert(cursor->keys.end(), rightNode->keys.begin(), rightNode->keys.end());
        cursor->ptr2next = rightNode->ptr2next;
        removeInternal(parent->keys[rightSibling - 1], parent, rightNode);
        delete rightNode;
    }
}

void BPlusTree::removeInternal(int x, Node* cursor, Node* child) {
    Node* root = getRoot();

    if (cursor == root) {
        if (cursor->keys.size() == 1) {
            if (cursor->ptr2TreeOrData.ptr2Tree[0] == child) {
                setRoot(cursor->ptr2TreeOrData.ptr2Tree[1]);
                delete cursor;
                return;
            } else if (cursor->ptr2TreeOrData.ptr2Tree[1] == child) {
                setRoot(cursor->ptr2TreeOrData.ptr2Tree[0]);
                delete cursor;
                return;
            }
        }
    }

    int pos;
    for (pos = 0; pos < cursor->keys.size(); pos++) {
        if (cursor->keys[pos] == x) {
            break;
        }
    }
    cursor->keys.erase(cursor->keys.begin() + pos);

    for (pos = 0; pos < cursor->ptr2TreeOrData.ptr2Tree.size(); pos++) {
        if (cursor->ptr2TreeOrData.ptr2Tree[pos] == child) {
            break;
        }
    }
    cursor->ptr2TreeOrData.ptr2Tree.erase(cursor->ptr2TreeOrData.ptr2Tree.begin() + pos);

    if (cursor->keys.size() >= (maxIntChildLimit + 1) / 2 - 1) {
        return;
    }

    if (cursor == root) {
        return;
    }

    Node** p1 = findParent(root, cursor);
    Node* parent = *p1;

    int leftSibling, rightSibling;

    for (pos = 0; pos < parent->ptr2TreeOrData.ptr2Tree.size(); pos++) {
        if (parent->ptr2TreeOrData.ptr2Tree[pos] == cursor) {
            leftSibling = pos - 1;
            rightSibling = pos + 1;
            break;
        }
    }

    if (leftSibling >= 0) {
        Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];

        if (leftNode->keys.size() >= (maxIntChildLimit + 1) / 2) {
            cursor->keys.insert(cursor->keys.begin(), parent->keys[leftSibling]);
            parent->keys[leftSibling] = leftNode->keys.back();
            leftNode->keys.pop_back();
            cursor->ptr2TreeOrData.ptr2Tree.insert(cursor->ptr2TreeOrData.ptr2Tree.begin(), leftNode->ptr2TreeOrData.ptr2Tree.back());
            leftNode->ptr2TreeOrData.ptr2Tree.pop_back();
            return;
        }
    }

    if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
        Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];

        if (rightNode->keys.size() >= (maxIntChildLimit + 1) / 2) {
            cursor->keys.push_back(parent->keys[rightSibling - 1]);
            parent->keys[rightSibling - 1] = rightNode->keys.front();
            rightNode->keys.erase(rightNode->keys.begin());
            cursor->ptr2TreeOrData.ptr2Tree.push_back(rightNode->ptr2TreeOrData.ptr2Tree.front());
            rightNode->ptr2TreeOrData.ptr2Tree.erase(rightNode->ptr2TreeOrData.ptr2Tree.begin());
            return;
        }
    }

    if (leftSibling >= 0) {
        Node* leftNode = parent->ptr2TreeOrData.ptr2Tree[leftSibling];
        leftNode->keys.push_back(parent->keys[leftSibling]);
        leftNode->keys.insert(leftNode->keys.end(), cursor->keys.begin(), cursor->keys.end());
        leftNode->ptr2TreeOrData.ptr2Tree.insert(leftNode->ptr2TreeOrData.ptr2Tree.end(), cursor->ptr2TreeOrData.ptr2Tree.begin(), cursor->ptr2TreeOrData.ptr2Tree.end());
        removeInternal(parent->keys[leftSibling], parent, cursor);
        delete cursor;
    } else if (rightSibling < parent->ptr2TreeOrData.ptr2Tree.size()) {
        Node* rightNode = parent->ptr2TreeOrData.ptr2Tree[rightSibling];
        cursor->keys.push_back(parent->keys[rightSibling - 1]);
        cursor->keys.insert(cursor->keys.end(), rightNode->keys.begin(), rightNode->keys.end());
        cursor->ptr2TreeOrData.ptr2Tree.insert(cursor->ptr2TreeOrData.ptr2Tree.end(), rightNode->ptr2TreeOrData.ptr2Tree.begin(), rightNode->ptr2TreeOrData.ptr2Tree.end());
        removeInternal(parent->keys[rightSibling - 1], parent, rightNode);
        delete rightNode;
    }
}

void BPlusTree::generateDOT(Node* cursor, ofstream &file) {
    if (cursor != NULL) {
        if (cursor->isLeaf) {
            file << "  node" << cursor << " [label=\"";
            for (int i = 0; i < cursor->keys.size(); i++) {
                file << cursor->keys[i];
                if (i < cursor->keys.size() - 1) file << "|";
            }
            file << "\", shape=record];\n";
        } else {
            file << "  node" << cursor << " [label=\"<f0>";
            for (int i = 0; i < cursor->keys.size(); i++) {
                file << "|<f" << i + 1 << ">" << cursor->keys[i];
            }
            file << "\", shape=record];\n";
        }

        if (!cursor->isLeaf) {
            for (int i = 0; i < cursor->ptr2TreeOrData.ptr2Tree.size(); i++) {
                if (cursor->ptr2TreeOrData.ptr2Tree[i] != NULL) {
                    file << "  node" << cursor << ":f" << i << " -> node" << cursor->ptr2TreeOrData.ptr2Tree[i] << ";\n";
                    generateDOT(cursor->ptr2TreeOrData.ptr2Tree[i], file);
                }
            }
        }
    }
}

void BPlusTree::generateDOTFile(const string& filename) {
    ofstream file;
    file.open(filename.c_str());
    file << "digraph BPlusTree {\n";
    file << "  node [shape=record];\n";
    generateDOT(root, file);
    file << "}\n";
    file.close();
}