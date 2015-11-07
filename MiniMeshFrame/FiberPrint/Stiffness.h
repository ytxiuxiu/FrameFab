#pragma once

#include <iostream>
#include <assert.h>

#include <Eigen/dense>
#include <Eigen/Sparse>
#include <Eigen/SparseCore>
#include <Eigen/SparseCholesky>
#include <Eigen/SparseLU>
#include <Eigen/SparseQR>
#include <Eigen/Core>
#include <Eigen/OrderingMethods>
#include <Eigen/IterativeLinearSolvers>

#include "WireFrame\WireFrame.h"
#include "DualGraph.h"
#include "FiberPrintPARM.h"
#include "Statistics.h"

using namespace std;
using namespace Eigen;


class Stiffness
{
public:
	typedef Eigen::SparseMatrix<double> SpMat;
	typedef Eigen::MatrixXd MX;
	typedef Eigen::Matrix3d M3;
	typedef Eigen::VectorXd VX;
	typedef Eigen::Vector3d V3;

	Stiffness();
	Stiffness(WireFrame *ptr_frame, DualGraph *ptr_dualgraph);
	Stiffness(WireFrame *ptr_frame, DualGraph *ptr_dualgraph, FiberPrintPARM *ptr_parm);
	~Stiffness();

public:
	void		CreateM();
	void		CreateK(const VectorXd *ptr_x);
	void		CreateFv();							// excute after CreateFe and CreateK
	void		CreateFv(const VectorXd *ptr_x);
	void		CreateFe();

	// Socket to GraphCut
	void		CalculateD(VectorXd *ptr_D, const VectorXd *ptr_x);

	// Data I/O
	SpMat		*WeightedK(){ assert(&K_);  return &K_;  }
	VectorXd	*WeightedF(){ assert(&Fv_); return &Fv_; }

	M3			Me(int ei);
	V3			Fe(int ei);

	void		Debug();

public:
	//private:
	WireFrame				*ptr_frame_;
	DualGraph				*ptr_dualgraph_;
	FiberPrintPARM			*ptr_parm_;

	int						Nk_ ;
	vector<int>				v_id_;					// tracking nodes to be deleted from stiffness matrix

	vector<Matrix3d>		M_;						// indexed by dual id
	SpMat					K_;						// x-Weighted global stiffness matrix, 
													// 3n*3n, n is the node number of orig graph
	VectorXd				Fv_;
	VectorXd				Fe_;
};