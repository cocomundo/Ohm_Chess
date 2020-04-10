#ifndef DIRECTION_H
#define DIRECTION_H


class Direction
{
    public:
        //Direction();
        inline int *dir_B(){return m_dir_B;}
        inline int *dir_R(){return m_dir_R;}
        inline int *dir_Q_Ki(){return m_dir_Q_Ki;}
        inline int *dir_Kn(){return m_dir_Kn;}
        inline int *dir_wP(){return m_dir_wP;}
        inline int *dir_bP(){return m_dir_bP;}

    protected:

    private:
        int m_dir_B[5]={4,-9,9,-11,11};
        int m_dir_R[5]={4,-1,1,-10,10};
        int m_dir_Q_Ki[9]={8,-1,1,-9,9,-10,10,-11,11};
        int m_dir_Kn[9]={8,12,-12,21,-21,8,-8,19,-19};
        int m_dir_wP[5]={4,-10,-20,-9,-11};
        int m_dir_bP[5]={4,10,20,9,11};
};

#endif // DIRECTION_H
