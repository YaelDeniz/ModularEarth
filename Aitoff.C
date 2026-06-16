// aitoff_coastlines.C
#include "TCanvas.h"
#include "TH2F.h"
#include "TStyle.h"
#include "TPolyLine.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "TCanvas.h"
#include "TPad.h"
#include "TH1F.h"
#include "TEllipse.h"
#include <cmath>

void DrawAitoffFrameNoAxes()
{
  auto* c = new TCanvas("cAitoff","Aitoff (ellipse only)", 1400, 700);
  c->SetLeftMargin(0.02);
  c->SetRightMargin(0.02);
  c->SetBottomMargin(0.02);
  c->SetTopMargin(0.08);

  // 1) Create a frame (sets coordinate system) but hide axes
  TH1F* frame = gPad->DrawFrame(-M_PI, -M_PI/2.0, M_PI, M_PI/2.0);
  frame->SetTitle("Earth surface (Aitoff projection)");

  frame->GetXaxis()->SetLabelSize(0);
  frame->GetXaxis()->SetTickLength(0);
  frame->GetXaxis()->SetTitle("");

  frame->GetYaxis()->SetLabelSize(0);
  frame->GetYaxis()->SetTickLength(0);
  frame->GetYaxis()->SetTitle("");

  // Optional: also hide the box around the pad/frame
  gPad->SetFrameLineWidth(0);
  gPad->SetFrameBorderMode(0);

  // 2) Draw the Aitoff boundary ellipse
  // Aitoff footprint is an ellipse with radii (pi, pi/2)
  auto* e = new TEllipse(0.0, 0.0, M_PI, M_PI/2.0);
  e->SetFillStyle(0);     // transparent
  e->SetLineColor(kBlack);
  e->SetLineWidth(2);
  e->Draw("same");

  c->Update();
}


static double WrapLonRad(double lon) {
  while (lon >  M_PI) lon -= 2.0*M_PI;
  while (lon < -M_PI) lon += 2.0*M_PI;
  return lon;
}

// lon/lat (deg) -> Aitoff (x,y) in radians
static void AitoffProjectDeg(double lon_deg, double lat_deg, double& x, double& y)
{
  const double lon = WrapLonRad(lon_deg * M_PI/180.0);
  const double lat = lat_deg * M_PI/180.0;

  const double alpha = std::acos(std::cos(lat) * std::cos(lon/2.0));
  if (std::abs(alpha) < 1e-12) { x = 0.0; y = lat; return; }

  const double sinc = std::sin(alpha) / alpha;
  x = 2.0 * std::cos(lat) * std::sin(lon/2.0) / sinc;
  y =        std::sin(lat) / sinc;
}

static void DrawGraticule()
{
  // Equator
  {
    std::vector<double> x, y;
    x.reserve(361); y.reserve(361);
    for (int i=0;i<=360;i++){
      double lon = -180.0 + i;
      double xx, yy;
      AitoffProjectDeg(lon, 0.0, xx, yy);
      x.push_back(xx); y.push_back(yy);
    }
    auto* eq = new TPolyLine((int)x.size(), x.data(), y.data());
    eq->SetLineColor(kGray+1);
    eq->SetLineWidth(1);
    eq->Draw("same");
  }

  // Prime meridian
  {
    std::vector<double> x, y;
    x.reserve(181); y.reserve(181);
    for (int i=0;i<=180;i++){
      double lat = -90.0 + i;
      double xx, yy;
      AitoffProjectDeg(0.0, lat, xx, yy);
      x.push_back(xx); y.push_back(yy);
    }
    auto* pm = new TPolyLine((int)x.size(), x.data(), y.data());
    pm->SetLineColor(kGray+1);
    pm->SetLineWidth(1);
    pm->Draw("same");
  }
}

static void DrawCoastlinesAitoff(const char* coast_file)
{
  std::ifstream in(coast_file);
  if (!in) {
    ::Error("DrawCoastlinesAitoff", "Cannot open %s", coast_file);
    return;
  }

  std::vector<double> xs, ys;
  xs.reserve(2000);
  ys.reserve(2000);

  double prev_lon = 0.0;
  bool have_prev = false;
  /*
  auto flush = [&]() {
    if (xs.size() >= 2) {
      auto* pl = new TPolyLine(xs.size(), xs.data(), ys.data());
      pl->SetLineColor(kBlack);
      pl->SetLineWidth(1);
      pl->Draw("same");
    }
    xs.clear();
    ys.clear();
    have_prev = false;
  };
  */
  auto flush = [&]() {
  if (xs.size() >= 2) {
    auto* pl = new TPolyLine(xs.size());
    for (int i = 0; i < (int)xs.size(); ++i) {
      pl->SetPoint(i, xs[i], ys[i]);  // copies the values into pl
    }
    pl->SetLineColor(kBlack);
    pl->SetLineWidth(1);
    pl->Draw("same");
  }
  xs.clear();
  ys.clear();
  have_prev = false;
  };


  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) {
      flush();
      continue;
    }

    std::istringstream ss(line);
    double lon_deg, lat_deg;
    if (!(ss >> lon_deg >> lat_deg)) continue;

    // Detect dateline jump BEFORE projecting
    if (have_prev && std::abs(lon_deg - prev_lon) > 180.0) {
      flush();
    }

    double x, y;
    AitoffProjectDeg(lon_deg, lat_deg, x, y);
    xs.push_back(x);
    ys.push_back(y);

    prev_lon = lon_deg;
    have_prev = true;
  }

  flush(); // final segment
}


void Aitoff(const char* coast_file = "coastlines_ll.txt")
{
  gStyle->SetOptStat(0);

  auto* c = new TCanvas("cAitoff", "Earth map (Aitoff) + coastlines", 1400, 700);

  // Aitoff output ranges
  auto* frame = new TH2F("frame",
                         "Earth surface (Aitoff projection);; ",
                         100, -M_PI,  M_PI,
                         100, -M_PI/2.0, M_PI/2.0);
  frame->SetStats(0);
  frame->Draw("AXIS");

  DrawGraticule();                 // optional
  DrawAitoffFrameNoAxes();
  DrawCoastlinesAitoff(coast_file);

  c->Update();
}
