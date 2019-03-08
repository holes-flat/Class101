/**
 * @file readmesh.cpp
 * @brief simply read data from @p easymesh
 * @author Yang Yichen, yichenyang@pku.edu.cn
 * @version 1
 * @date 2019-03-08
 */

#include <EasyMesh.h>
#include <iostream>

/// We list what AFEPack really read.
/**
 * First for ".n" file (Nodes' information).
 * <ul>
 *     <li> this->Mesh<2,DOW>::point(i)
 *     <li> this->Mesh<2,DOW>::boundaryMark(0,i)
 *     <li> this->Mesh<2,DOW>::geometry(0,i).index()
 *     <li> this->Mesh<2,DOW>::geometry(0,i).vertex()   null
 *     <li> this->Mesh<2,DOW>::geometry(0,i).boundary()   null
 * </ul>
 *
 * Second for ".s" file (sides' information).
 * <ul>
 *     <li> this->Mesh<2,DOW>::boundaryMark(1,i)
 *     <li> this->Mesh<2,DOW>::geometry(1,i).index()
 *     <li> this->Mesh<2,DOW>::geometry(1,i).vertex(0)  .vertex(1)
 *     <li> this->Mesh<2,DOW>::geometry(1,i).boundary() = .vertex()
 * </ul>
 *
 * Third for ".e" file (elements' information).
 * <ul>
 *     <li> this->Mesh<2,DOW>::boundaryMark(2,i) = 0
 *     <li> this->Mesh<2,DOW>::geometry(2,i).vertex(0-2)
 *     <li> this->Mesh<2,DOW>::geometry(2,i).boundary(0-2)
 * </ul>
 *
 * WARNING: Vertices in geometry(2,i) will be ordered as counterclockwise.
 */

int main(int argc, char ** argv){
	EasyMesh newmesh;
	newmesh.readData(argv[1]);
}
