/**
 * @file possion.cpp
 * @brief First example of FEM method.
 * @author Yang Yichen, yichenyang@pku.edu.cn
 * @version 1
 * @date 2019-03-09
 */

#include <AFEPack/AMGSolver.h>
#include <AFEPack/TemplateElement.h>
#include <AFEPack/FEMSpace.h>
#include <AFEPack/Operator.h>
#include <AFEPack/BilinearOperator.h>
#include <AFEPack/Functional.h>
#include <AFEPack/EasyMesh.h>

#define PI M_PI

/**
 * @brief 精确解　\f$ u(x,y) = \sin(\pi x)\sin(2\pi y) \f$.
 */
double u(const double *p){
	return sin(PI*p[0]) * sin(2*PI*p[1]) + p[0];
}

/**
 * @brief 右端项  \f$ f(x,y) = 5\pi^2 u(x,y) \f$.
 */
double f(const double *p){
	return 5*PI*PI*(u(p)-p[0]);
}

int main(int argc, char **argv){
	/**
	 * 读入网格
	 */
	EasyMesh mesh;
	mesh.readData(argv[1]);

	/**
	 * 下面构造有限元空间，包括四步:
	 *
	 * 		- 参考单元的几何信息
	 * 		- 参考单元到网格中单元的坐标变换
	 * 		- 单元上的自由度分布
	 * 		- 单元的基函数
	 *
	 * 	若使用AFEPack自带的template模板，必须引入环境变量，例如
	 *	<pre>
	 *   	$ export AFEPACK_TEMPLATE_PATH=$AFEPACK_PATH/template/triangle
	 * 	</pre>
	 */

	/**
	 * @brief 参考单元的几何信息。
	 *
	 * 		- 参考单元的几何结构
	 * 		- 数值积分公式
	 */
	TemplateGeometry<2> triangle_template_geometry;
	triangle_template_geometry.readData("triangle.tmp_geo");
	
	/**
	 * @brief 参考单元到网格单元的坐标变换。
	 *
	 * 		- 变换
	 * 		- 变换的雅可比行列式
	 */
	CoordTransform<2,2> triangle_coord_transform;
	triangle_coord_transform.readData("triangle.crd_trs");

	/**
	 * @brief 参考单元上自由度的分布。
	 *
	 * 		- 自由度个数
	 */
	TemplateDOF<2> triangle_template_dof(triangle_template_geometry);
	triangle_template_dof.readData("triangle.1.tmp_dof");

	/**
	 * @brief  基函数相关信息。
	 */
	BasisFunctionAdmin<double,2,2> triangle_basis_function(triangle_template_dof);
	triangle_basis_function.readData("triangle.1.bas_fun");

	
	/**
	 * @brief 初始化一个参考单元。
	 */
  	std::vector<TemplateElement<double,2,2> > template_element(1);
  	template_element[0].reinit(triangle_template_geometry,
			     triangle_template_dof,
			     triangle_coord_transform,
			     triangle_basis_function);

	/**
	 * @brief 初始化有限元空间。
	 */
  	FEMSpace<double,2> fem_space(mesh, template_element);
	int n_ele = mesh.n_geometry(2);
	fem_space.element().resize(n_ele);
	for (int i = 0; i < n_ele; i++)
		fem_space.element(i).reinit(fem_space,i,0);
	
	fem_space.buildElement();
	fem_space.buildDof();
	fem_space.buildDofBoundaryMark();

	/**
	 * 计算刚度矩阵，库当中有准备这个矩阵。
	 *
	 * 		- 构造稀疏矩阵
	 */
	StiffMatrix<2,double> stiff_matrix(fem_space);
	stiff_matrix.algebricAccuracy() = 6; 
	stiff_matrix.build();
	
	FEMFunction<double,2> u_h(fem_space);

	Vector<double> f_h;
	Operator::L2Discretize(&f, fem_space, f_h, 6);
	
	/**
	 * Dirichlet 边界条件，根据材料标识给出。
	 */
	BoundaryFunction<double,2> boundary(BoundaryConditionInfo::DIRICHLET,
			1,&u);
	BoundaryConditionAdmin<double,2> boundary_admin(fem_space);
	boundary_admin.add(boundary);
	boundary_admin.apply(stiff_matrix, u_h, f_h);

	/**
	 * 代数多重网格求解。
	 */
	AMGSolver solver(stiff_matrix);
	solver.solve(u_h, f_h);

	/// 输出解，画图
	/**
	 * 对Tecplot输出的源码格式做了小修改。
	 */
	u_h.writeTecplotData("u.dat");

	/// 输出误差
	double error = Functional::L2Error(u_h, FunctionFunction<double>(&u), 3);
  	std::cerr << "\nL2 error = " << error << std::endl;

	return 0;
}

