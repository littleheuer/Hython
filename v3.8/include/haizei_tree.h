/*************************************************************************
	> File Name: ASTree.h
	> Author: 
	> Mail: 
	> Created Time: 日  3/29 15:36:27 2020
 ************************************************************************/

#ifndef _ASTREE_H
#define _ASTREE_H
#include <string>
#include <hlLexer.h>
#include <hlParser.h>
#include <haizei_master.h>
#include <vector>

namespace haizei {

class ASTree {
public :
    //转换构造函数，将抽象语法树转换为我们的树结构
    ASTree(pANTLR3_BASE_TREE tree);
    int size() const;
    //获取第i棵子树
    ASTree *at(int ind);
    //当前节点字符串信息 
    std::string text() const;
    int type() const;
    //每个节点挂一个可处理当前节点的工厂 
    IMaster::IFactory *factory;

private:
    //抽象语法树
    pANTLR3_BASE_TREE tree;
    //节点类型，子节点数量
    int __type, __size;
    //子树
    std::vector<ASTree *> child;
};

}
#endif
