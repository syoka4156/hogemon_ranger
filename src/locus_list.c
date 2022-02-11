#include <stdio.h>
#include <stdlib.h>
#include "locus_list.h"


void init_list(linked_list *list) {
    list->root = NULL;
    list->size = 0;
}

// リストを再設定
void reset_list(linked_list *list, int *PREV_X, int *PREV_Y) {
    *PREV_X = -10;
    *PREV_Y = -10;

    // 先頭から順にノードをfreeしていく
    list_node *iter = list->root;
    while (iter != NULL) {
        // 先に次のノードのポインタを別の変数にコピーしてからfreeする
        list_node *next = iter->next;
        free(iter);
        iter = next;
    }

    // リストの初期化
    init_list(list);      
}

// rootを先頭要素とする連結リストの末尾に新しいノードを追加
void add_node(linked_list *list, int x, int y) {
    // 新しいノードの作成
    list_node *node = (list_node*)malloc(sizeof(list_node));
    node->x = x;
    node->y = y;
    node->next = NULL;

    // 末尾要素を探す
    if (list->root == NULL) {
        // まだ要素がなければ，先頭要素を新規ノードにする
        list->root = node;
    } else {
        // あるノードの次のノード (next) がNULLになるものを探す
        list_node *iter = list->root;
        while (iter->next != NULL) {
            iter = iter->next;
        }

        // 見つかったら末尾要素を新規ノードで置き換える
        iter->next = node;
    }

    // リストの要素数を1増やす
    list->size += 1;
}

// リストのデータを全て削除する
void free_list(linked_list *list) {
    // 先頭から順にノードをfreeしていく
    list_node *iter = list->root;
    while (iter != NULL) {
        // 先に次のノードのポインタを別の変数にコピーしてからfreeする
        list_node *next = iter->next;
        free(iter);
        iter = next;
    }

    // 最後にリスト自体をfreeする
    free(list);
}