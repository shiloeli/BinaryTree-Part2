#pragma once
#include <iostream>
#include <exception>
#include <vector>
#include <string>

namespace ariel{
    template<typename T>class BinaryTree{

        private:
            class Node{
                public:
                    T data;
                    Node *left, *right;

                    Node(const T d, Node* l=nullptr, Node* r=nullptr):data(d),left(l),right(r){}

                    Node(Node* node){
                        this = node;
                    }

                    Node(const Node* node){
                        if(node!= nullptr){
                            this->data = node->data;
                            this->left = node->left;
                            this->right = node->right;
                        }
                    }
            };
        
            Node* root;

        public:
            BinaryTree():root(nullptr){}

            BinaryTree(const BinaryTree& tree){
                if(tree.root){
                    this->root = new Node(tree.root->data);
                    copy(root,tree.root);
                }
            }

            BinaryTree(const BinaryTree&& tree)noexcept{
                this->root = tree.root;
            }

            ~BinaryTree(){
                delete_tree(root);
            }

            void delete_tree(Node* node){
                if(node){
                delete_tree(node->right);
                delete_tree(node->left);
                delete node;
                }
            }

            Node* search_node(Node* node, const T& d_parent){
                if(!node){return nullptr;}
                if(node->data == d_parent){return node;}

                Node* left = search_node(node->left, d_parent);
                if(left){return left;}
                
                Node* right = search_node(node->right, d_parent);
                return right;
            }

            BinaryTree& add_root(T r_data){
                if(!root){root = new Node(r_data);}
                else{ root->data = r_data;}
                return *this;
            }

            BinaryTree& add_left(const T p_data, const T l_data){
                Node* node = search_node(this->root,p_data);
                if(!node){
                    std::__throw_invalid_argument("The data of parent no is no contained in the tree");
                }
                if(!node->left){
                    node->left = new Node(l_data);
                }
                node->left->data = l_data;
                return *this;
            }

            BinaryTree& add_right(const T& p_data, const T& r_data){
                Node* node = search_node(this->root,p_data);
                if(!node){
                    std::__throw_invalid_argument("The data of parent no is no contained in the tree");
                }
                if(!node->right){
                    node->right = new Node(r_data);
                }
                node->right->data = r_data;
                return *this;
            }

            void copy(Node* root1, const Node* root2)const{
                if(!root2){return;}
                    root1->data = root2->data;
                    if(root2->left){
                    root1->left = new Node(root2->left->data);
                    }
                    if(root2->right){
                    root1->right = new Node(root2->right->data);
                    }
                    copy(root1->left,root2->left);
                    copy(root1->right, root2->right);     
            }

        BinaryTree& operator=(BinaryTree tree){
            if(!tree.root){
                return *this;
            }
            root = new Node(tree.root->data);
            copy(root, tree.root);
            return *this;
        }

        BinaryTree& operator=(BinaryTree&& tree)noexcept{
            if(!tree.root){
                return *this;
            }
            root = tree.root;
            return *this;
        }

            class pre_iterator{
                private:
                    Node* pointer_node;
                    Node* root;
                    std::vector<Node*> store_node;
                
                public:
                    pre_iterator(Node* pointer = nullptr):pointer_node(pointer){build_store(pointer);}

                    void build_store(Node* node){
                        if(!node){
                            return;
                        }
                        store_node.push_back(node);
                        build_store(node->left);
                        build_store(node->right);
                        }

                    pre_iterator& operator++(){
                        if(store_node.size()<=1){
                            if(store_node.size()==1){
                            store_node.erase(store_node.begin());
                        }
                        pointer_node = nullptr;
                        }else{
                            store_node.erase(store_node.begin());
                            this->pointer_node = store_node[0];
                        }
                        return *this;
                    } 

                    pre_iterator operator++(int){
                        pre_iterator tmp = *this;
                        if(store_node.size()<=1){
                            if(store_node.size()==1){
                            store_node.erase(store_node.begin());
                        }
                        pointer_node = nullptr;
                        }else{
                            store_node.erase(store_node.begin());
                            this->pointer_node = store_node[0];
                        }
                        return tmp;
                    }

                    T& operator*()const{
                        return pointer_node->data;
                    }

                    T* operator->()const{
                        return &(pointer_node->data);
                    }

                    bool operator==(const pre_iterator& preorder)const{
                            return preorder.pointer_node == this->pointer_node;
                    }

                    bool operator!=(const pre_iterator& preorder)const{
                        return !(preorder.pointer_node==this->pointer_node);
                    }
            };    

            class ino_iterator{
                private:
                    Node* pointer_node;
                    Node* root;
                    std::vector<Node*> store_node;

                public:

                    ino_iterator(Node* pointer = nullptr):pointer_node(nullptr),root(pointer){build_store(pointer);}

                    void build_store(Node* node){
                        if(!node){
                            if(!store_node.empty()){
                                pointer_node = store_node[0];
                            }
                            return;
                        }
                        build_store(node->left);
                        store_node.push_back(node);
                        build_store(node->right);
                    }

                    ino_iterator& operator++(){
                        if(store_node.size()>1){
                            store_node.erase(store_node.begin());
                            this->pointer_node = store_node[0];
                        }else{
                            if(store_node.size()==1){
                                store_node.erase(store_node.begin());
                            }
                            pointer_node = nullptr;
                        }
                        return *this;
                    }
                      
                    ino_iterator operator++(int){
                        ino_iterator tmp = *this;
                        if(store_node.size()>1){
                            store_node.erase(store_node.begin());
                            this->pointer_node = store_node[0];
                        }else{
                            if(store_node.size()==1){
                                store_node.erase(store_node.begin());
                            }
                            pointer_node = nullptr;
                        }
                        return tmp;
                    }

                    T& operator*()const{
                        return pointer_node->data;
                    }

                    T* operator->()const{
                        return &(pointer_node->data);
                    }

                    bool operator==(const ino_iterator& preorder)const{
                            return preorder.pointer_node == this->pointer_node;
                    }

                    bool operator!=(const ino_iterator& preorder)const{
                        return !(preorder.pointer_node==this->pointer_node);
                    }
            };    

            class post_iterator{
                private:
                    Node* pointer_node;
                    std::vector<Node*> store_node;

                public:

                    post_iterator(Node* pointer = nullptr){
                        build_store(pointer);
                         if(!store_node.empty()){
                                pointer_node = store_node[0];
                            }else{
                                pointer_node = nullptr;
                            }
                        
                        }

                    void build_store(Node* node){
                           if(!node){
                            return;
                        }
                        build_store(node->left);
                        build_store(node->right);
                        store_node.push_back(node);
                    }

                    post_iterator& operator++(){
                        if(store_node.size()<=1){
                            pointer_node = nullptr;
                            return *this;
                        }
                        store_node.erase(store_node.begin());
                        pointer_node = store_node[0];
                        return *this;
                    } 

                    post_iterator operator++(int){
                        post_iterator tmp = *this;
                        if(store_node.size()<=1){
                            pointer_node = nullptr;
                            return tmp;
                        }
                        store_node.erase(store_node.begin());
                        pointer_node = store_node[0];
                        return tmp;
                    }

                    T& operator*()const{
                        return pointer_node->data;
                    }

                    T* operator->()const{
                        return &(pointer_node->data);
                    }

                    bool operator==(const post_iterator& preorder)const{
                            return preorder.pointer_node == this->pointer_node;
                    }

                    bool operator!=(const post_iterator& preorder)const{
                        return !(preorder.pointer_node==this->pointer_node);
                    }
            };                

        pre_iterator begin_preorder(){return pre_iterator{root};}

        pre_iterator end_preorder(){return pre_iterator{nullptr};}

        ino_iterator begin_inorder(){return ino_iterator{root};}

        ino_iterator end_inorder(){return ino_iterator{nullptr};}

        post_iterator begin_postorder(){return post_iterator{root};}

        post_iterator end_postorder(){return post_iterator{nullptr};}

        ino_iterator begin(){return ino_iterator{root};}
                     
        ino_iterator end(){return ino_iterator{nullptr};}      

        static void printtree(const std::string& str, const Node* node, bool ileft){
            if( node != nullptr ){
                std::cout << str;
                std::cout << (ileft ? "|---" : "└---" );
                std::cout << node->data << std::endl;
                printtree( str + (ileft ? "│   " : "    "), node->left, true);
                printtree( str + (ileft ? "│   " : "    "), node->right, false);
            }
        }

        friend std::ostream &operator<<(std::ostream &out, const BinaryTree<T>& treeree){
            printtree("",treeree.root,false);
            return out;
        }      
    };
}






