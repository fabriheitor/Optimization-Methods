void calcular_FO(Solucao& s){
  s.fo = N;
  for(int i = 0; i < N; i++)
  {
    int id = (i * M) + s.vet_sol[i];
    for(int j = 0; j < vet_qtd_conf[id]; j++)
    {
        int id2 = mat_conf[id][j];
        int pt = id2 / M;
        int pos = id2 % M;
        if(s.vet_sol[pt] == pos)
        {
            s.fo--;
            break;
        }
    }
  }

}