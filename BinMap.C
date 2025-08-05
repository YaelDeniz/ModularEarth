#include <TCanvas.h>
#include <TPolyLine.h>
#include <TText.h>
#include <TLine.h>
#include <TMath.h>
#include <vector>
#include <iostream>

void BinMap() {
    const int Nphi = 12;     // 30-degree azimuthal bins
    const int Nth  = 9;      // 20-degree zenith bins

    const double dphi = 360.0 / Nphi;
    const double dth  = 180.0 / Nth;

    TCanvas* c = new TCanvas("c", "Polar Earth Bins", 800, 800);
    c->SetTheta(90);  // Top-down view
    c->Range(-1.2, -1.2, 1.2, 1.2);

    int id = 1;
    for (int ith = 0; ith < Nth; ++ith) {
        double th1 = ith * dth;
        double th2 = (ith + 1) * dth;
        double r1 = th1 / 180.0;  // Normalize θ to [0,1]
        double r2 = th2 / 180.0;

        for (int iph = 0; iph < Nphi; ++iph) {
            double phi1 = iph * dphi;
            double phi2 = (iph + 1) * dphi;

            const int Nseg = 30; // for smooth arcs
            std::vector<double> x, y;

            // Outer arc
            for (int s = 0; s <= Nseg; ++s) {
                double phi = phi1 + (phi2 - phi1) * s / Nseg;
                double rad = r2;
                x.push_back(rad * cos(phi * M_PI / 180));
                y.push_back(rad * sin(phi * M_PI / 180));
            }
            // Inner arc (reversed)
            for (int s = Nseg; s >= 0; --s) {
                double phi = phi1 + (phi2 - phi1) * s / Nseg;
                double rad = r1;
                x.push_back(rad * cos(phi * M_PI / 180));
                y.push_back(rad * sin(phi * M_PI / 180));
            }

            TPolyLine* poly = new TPolyLine(x.size(), &x[0], &y[0]);
            poly->SetLineColor(kBlack);
            poly->SetFillColor(kAzure + (id % 4));
            poly->SetFillStyle(1001);
            poly->Draw("f");

            // Optional: Add bin label at center
            double midphi = (phi1 + phi2) / 2 * M_PI / 180;
            double midr   = (r1 + r2) / 2;
            TText* t = new TText(midr * cos(midphi), midr * sin(midphi), Form("%d", id++));
            t->SetTextSize(0.02);
            t->SetTextAlign(22);
            t->Draw();
        }
    }

    c->Update();
}

