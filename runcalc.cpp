#include <iostream>
#include <vector>
using namespace std;

// CONTROL PANEL!
const bool exclude_long_runs = false;
const double long_run_cutoff = 10;		// 10 kilometers is my cutoff

	// report_mode = 1: report based on # of days since beginning
	// report_mode = 2: report based on # of runs since beginning
	// report_mode = 3: report based on # of miles since beginning
const int report_mode = 3;

const bool debug_mode = false;
// no changes below here for newb-mode. But feel free to hack & improve!

int main() {
	double dayCount=0, day, dist, kph, totalDist=0;
	vector<double> days, dists, totalDists, kphs;
	double junk; 
	while (cin >> day >> dist >> kph) {
		if (debug_mode) cout << "day = " << day << "; dist = " << dist << "; speed = " << kph << endl;
		totalDist += dist;
		if (!exclude_long_runs || dist < long_run_cutoff) {
			if (report_mode == 2) 
				days.push_back(dayCount++);
			else
				days.push_back(day);
			dists.push_back(dist);
			totalDists.push_back(totalDist);
			kphs.push_back(kph);
		}
	}

	// linear regression.
	// f(x) = Ax + B
	// error is E2 = sum_k^n |f(x_k) - y_k|^2
	// 				=sum_k^n (Ax_k+B - y_k)^2
	// 				=sum_k^n A^2x_k^2 + ABx_k - Ax_ky_k + BAx_k + B^2 - By_k -Ax_ky_k - By_k - y_k^2
	// 	Want to find min E2
	// 	ergo, take partial derivs:
	// 	\pd{E2}{A} = sum_k^n 2Ax_k^2 + 2Bx_k - 2x_ky_k = sum_k^n 2(Ax_k + B - y_k)x_k == 0
	// 	\pd{E2}{B} = sum_k^n 2Ax_k+ 2B - y_k - y_k   = sum_k^n 2(Ax_k + B - y_k) == 0
	// 	rewrite: cancel 2's. move to other side. Eq to satisfy is:
	// 		/	sum_k^n x_k^2		sum_k^n x_k		\/A\ = 	/ sum_k^n y_kx_k \
	//		\	sum_k^n x_k			sum_k^n 1		/\B/ = 	\ sum_k^n y_k	 /
	//	ergo, calculate each quantity:
	int N = days.size();
	string xunit;
	vector<double> xk;
	switch (report_mode) {
	case 1:
		xunit = "day";
		xk=days;
		break;
	case 2:
		xunit = "run";
		xk=totalDists;
		break;
	case 3:
		xunit = "kilometer";
		xk=totalDists;
		break;
	}
	vector<double> yk=kphs;
	double sumxk2=0.0, sumxk=0.0, sum1=N+0.0, sumykxk=0.0, sumyk=0.0;
	for (int i=0; i<N; i++) {
		if (debug_mode) cout << "xk[i=" << i << "] = " << xk[i] << "; yk[i=" << i << "]="<<yk[i] << endl;
		sumxk2 += xk[i]*xk[i];
		sumxk += xk[i];
		sumykxk += xk[i]*yk[i];
		sumyk += kphs[i];
	}

	double det = -sumxk2*sum1 + sumxk*sumxk;
	double A = -sum1*sumykxk + sumxk*sumyk;
	double B = sumxk*sumykxk - sumxk2*sumyk;
	A /= det;
	B /=det;
	if (debug_mode) {
		cout << "matrix to be solved: " << endl;
		cout << "[ " << sumxk2 << "  " << sumxk << " ][A] = [ " << sumykxk << " ]" << endl;
		cout << "[ " << sumxk << "  " << sum1 << " ][B] = [ " << sumyk << " ]" << endl;
	
		cout << "solution is: A = " << A << endl;
		cout << "             B = " << B << endl;
		cout << "In words, ";
	}
	cout << "I am improving at a rate of " << A*1000 << " meters/hr per " << xunit << ", or" << endl
			<< A/B*100 << "% per " << xunit << "." << endl << endl ;

	double xLeft = (16.09344-B)/A;
	cout << "Furthermore, projecting the same rate of increase can be sustained, "<< endl
		<< " I will hit the 6 minute/mile pinacle in " << xLeft << " " << xunit << "s of running," << endl
		<< "or approximately " << xLeft-xk.size() << " " << xunit << "s of running from now." <<  endl;

	return 0;
}
