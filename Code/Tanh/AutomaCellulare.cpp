#include "AutomaCellulare.hpp"
#include <iostream>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TF1.h>

void Revolution(double const G,double const g,double const Ag,double const alfa,double const beta,double const Nr0,double const Np0,double const R0,double const AR0, int tf,TMultiGraph* mg){
    double const h{1e-2}; 
   std::vector<double> Nr{Nr0/(Nr0+Np0+R0+AR0)},Np{Np0/(Nr0+Np0+R0+AR0)},R{R0/(Nr0+Np0+R0+AR0)},AR{AR0/(Nr0+Np0+R0+AR0)};
   std::vector<double> k1(4,0), k2(4,0), k3(4,0), k4(4,0);
   std::vector<double> x={Nr[0],Np[0],R[0],AR[0]}; 
   std::vector<double> t;
   double FR{Nr[0]+AR[0]}, FP{Np[0]+R[0]};
   std::cout<<"G:"<<G<<"g:"<<g<<"Ag:"<<"beta:"<<beta<<"alfa:"<<alfa<<"\nt Nr Np R AR\n";
   //std::fstream fout;
   //fout.open("Output.txt", std::fstream::out);
   for(int i{0};i<tf/h;++i)
   {
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
    TGraph* gr1 = new TGraph(tf/h,tA,RA);
    TGraph* gr2 = new TGraph(tf/h,tA,ARA);
    gr1->SetLineColor(2);
    gr2->SetLineColor(4);
    gr1->SetLineWidth(4);
    gr2->SetLineWidth(4);
    mg->Add(gr1);
    mg->Add(gr2);
    mg->Draw("L");

    TLegend* leg = new TLegend(0.68, 0.61, .89, .71);
    leg->AddEntry(gr1,"Rivoluzionari","l");
    leg->AddEntry(gr2,"Reazionari","l");
    leg->Draw("SAME");

    TPaveText *pt = new TPaveText(0.58, 0.35, .89, .59,"NDC");
    pt->AddText("Costanti della Simulazione");
    pt->AddText(Form("#Gamma=%g  #gamma=%g  #bar{#gamma}=%g",G, g,Ag));
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

void automa(){
    const int day{100};
    double const G{0.4},
             g{0.4},
             Ag{0.6},
             beta{0.4},
             alfa{0.6},
             Nr{2000},
             Np{8000},
             R0{0},
             AR0{0},
             f{8};
    Nation n1{Nr,Np,R0,AR0,G/f,g,Ag/f,beta/f,alfa/f};
    std::vector<double> t,R,AR;
    n1.printGrid();
    t.push_back(0);
        R.push_back(n1.countTotal(State::REVOL)/(Nr+AR0+Np+R0));
        AR.push_back(n1.countTotal(State::REACT)/(Nr+AR0+Np+R0));
    for(double i{1};i!=day+1;++i){
        n1.evolve();
        std::cout<<i<<' '<<n1.countTotal(State::REVOL)/(Nr+AR0+Np+R0)<<' '<<n1.countTotal(State::REACT)/(Nr+AR0+Np+R0)<<std::endl;
        t.push_back(i);
        R.push_back(n1.countTotal(State::REVOL)/(Nr+AR0+Np+R0));
        AR.push_back(n1.countTotal(State::REACT)/(Nr+AR0+Np+R0));
        n1.printGrid();
    }

    double tA[t.size()];
    double RA[R.size()];
    double ARA[AR.size()];
    std::copy(t.begin(), t.end(), tA);
    std::copy(R.begin(), R.end(), RA);
    std::copy(AR.begin(), AR.end(), ARA);
    TMultiGraph *mg = new TMultiGraph("multiGraph","Integrazione modello rivoluzione");
    mg->SetTitle("Integrazione modello rivoluzione;Tempo [Giorno];Frazione Persone");
    TGraph* gr1 = new TGraph(day,tA,RA);
    TGraph* gr2 = new TGraph(day,tA,ARA); 
    //TF1* fInitz = new TF1("Approx","[0]*e^([1]*x)",0,3);
    //fInitz->SetParameters(0.7,0.001);
    //gr1->Fit("Approx","R");
    
    gr1->SetMarkerColor(2);
    gr2->SetMarkerColor(4);
    gr1->SetMarkerSize(4);
    gr2->SetMarkerSize(4);
    mg->Add(gr1);
    mg->Add(gr2);
    mg->Draw("AP");
    //gr1->GetFunction("Approx")->Draw("SAME");
   

    TLegend* leg = new TLegend(0.68, 0.61, .89, .71);
    leg->AddEntry(gr1,"Rivoluzionari","p");
    leg->AddEntry(gr2,"Reazionari","p");
    leg->Draw("SAME");

    Revolution(G,g,Ag,alfa,beta,Nr,Np,R0,AR0,day,mg);

}