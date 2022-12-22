#include <math.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TPaveText.h>




void Revolution(){
    double const h{1e-2},
             pilp{.5},
             Gp{0.5},
             gp{0.5},
             Agp{0.5},
             beta{0.4},
             alfa{0.8},
             tf{3000};
   std::vector<double> Nr{.5},Np{.5},R{0},AR{0};
   std::vector<double> k1(4,0), k2(4,0), k3(4,0), k4(4,0);
   std::vector<double> x={Nr[0],Np[0],R[0],AR[0]}; 
   std::vector<double> t;
   double FR{Nr[0]+AR[0]}, FP{Np[0]+R[0]},sus{1-exp(-FP/FR)};
   for(int i{0};i<tf/h;++i)
   {
    double pil{pilp},G{Gp*tanh(sus*pil/300)},g{gp*tanh(sus*pil)},Ag{Agp*tanh(1/pil/sus)};
    k1[2]=G*x[2]*x[1]+g*x[1]-Ag*x[2]*x[3];
    k1[3]=alfa*x[2]*x[0]-beta*x[1]*x[3];
    k1[1]=-k1[2];
    k1[0]=-k1[3];
    k2[2]=G*(x[2]+h/2*k1[2])*(x[1]+h/2*k1[1])+g*(x[1]+h/2*k1[1])-Ag*(x[2]+h/2*k1[2])*(x[3]+h/2*k1[3]);
    k2[3]=alfa*(x[2]+h/2*k1[2])*(x[0]+h/2*k1[0])-beta*(x[1]+h/2*k1[1])*(x[3]+h/2*k1[3]);
    k2[1]=-k2[2];
    k2[0]=-k2[3];
    k3[2]=G*(x[2]+h/2*k2[2])*(x[1]+h/2*k2[1])+g*(x[1]+h/2*k2[1])-Ag*(x[2]+h/2*k2[2])*(x[3]+h/2*k2[3]);
    k3[3]=alfa*(x[2]+h/2*k2[2])*(x[0]+h/2*k2[0])-beta*(x[1]+h/2*k2[1])*(x[3]+h/2*k2[3]);
    k3[1]=-k3[2];
    k3[0]=-k3[3];
    k4[2]=G*(x[2]+h*k3[2])*(x[1]+h*k3[1])+g*(x[1]+h*k3[1])-Ag*(x[2]+h*k3[2])*(x[3]+h*k3[3]);
    k4[3]=alfa*(x[2]+h*k3[2])*(x[0]+h*k3[0])-beta*(x[1]+h*k3[1])*(x[3]+h*k3[3]);
    k4[1]=-k4[2];
    k4[0]=-k4[3];
    std::cout<<i*h<<' ';
    for(int j{0};j!=4;++j){
        x[j]+=(k1[j]+2*k2[j]+2*k3[j]+k4[j])*h/6;
        if(x[j]<0) x[j]=0;
    }
    if(x[0]>Nr[0]){
        x[0]-=x[0]-Nr[0];
    }
    if(x[1]>Np[0]){
        x[1]-=x[1]-Np[0];
    }
    if(x[3]>Nr[0]){
        x[3]-=x[3]-Nr[0];
    }
    if(x[2]>Np[0]){
        x[2]-=x[2]-Np[0];
    }
    for(int j{0};j!=4;++j){
        std::cout<<x[j]<<' ';
    }
    std::cout<<'\n';
    Nr.push_back(x[0]);
    Np.push_back(x[1]);
    R.push_back(x[2]);
    AR.push_back(x[3]);
    t.push_back(i*h);
   }
    double tA[t.size()];
    double RA[R.size()];
    double ARA[AR.size()];
    std::copy(t.begin(), t.end(), tA);
    std::copy(R.begin(), R.end(), RA);
    std::copy(AR.begin(), AR.end(), ARA);
    TMultiGraph *mg = new TMultiGraph("multiGraph","Integrazione modello rivoluzione");
    mg->SetTitle("Integrazione modello rivoluzione;Tempo [Giorno];Frazione Persone");
    TGraph* gr1 = new TGraph(tf/h,tA,RA);
    TGraph* gr2 = new TGraph(tf/h,tA,ARA);
    gr1->SetLineColor(2);
    gr2->SetLineColor(4);
    gr1->SetLineWidth(4);
    gr2->SetLineWidth(4);
    mg->Add(gr1);
    mg->Add(gr2);
    mg->Draw("AL");

    TLegend* leg = new TLegend(0.68, 0.61, .89, .71);
    leg->AddEntry(gr1,"Rivoluzionari","l");
    leg->AddEntry(gr2,"Reazionari","l");
    leg->Draw("SAME");

    TPaveText *pt = new TPaveText(0.58, 0.35, .89, .59,"NDC");
    pt->AddText("Costanti della Simulazione");
    pt->AddText(Form("#Gamma_{0}=%g  #gamma_{0}=%g  #bar{#gamma}_{0}=%g",Gp, gp,Agp));
    pt->AddText(Form("#alpha=%g  #beta=%g  F_{r}=%g  F_{p}=%g",alfa, beta, FR, FP));
    pt->SetFillColor(0);
    pt->SetBorderSize(4);
   // pt->AddLine(.0,0,0.,1);
    pt->AddLine(.0,-0.01,1.,-0.01);
    pt->AddLine(.0,1,1.,1);
    pt->AddLine(1,-0.01,1.,1);
    pt->AddLine(.0,1,.000000000000000000000000000001,-0.01);

   pt->Draw("SAME");
}