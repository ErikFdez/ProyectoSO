using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace Cliente
{
    public partial class Form1 : Form
    {

        public bool conectado = false;
        public int ID_J;
        Socket server;

        public Form1()
        {
            InitializeComponent();
        }


        private void btnDesconectar_Click(object sender, EventArgs e)
        {
            username.Enabled = true;
            password.Enabled = true;
            username.Visible = false;
            password.Visible = false;
            label1.Visible = false;
            label2.Visible = false;
            MostrarPass.Visible = false;
            BtnRegistrar.Visible = false;
            BtnLogin.Visible = false;         
            btnDesconectar.Visible = false;
            label5.Visible = false;
            label6.Visible = false;
            peticion.Visible = false;
            dataGridView1.Visible = false;
            btn_lista_conectados.Visible = false;
            btnConectar.Enabled = true;
            numConsultas.Visible = false;
            contConsultas.Visible = false;
            //Mensaje de desconexión
            string mensaje = "0/";

                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

            // Nos desconectamos
                username.Clear();
                password.Clear();
                server.Shutdown(SocketShutdown.Both);
                server.Close();
                MessageBox.Show("Desconectado correctamente, vuelve pronto");

        }


        private void btnConectar_Click(object sender, EventArgs e)
        {
            username.Visible = true;
            password.Visible = true;
            BtnRegistrar.Enabled = true;
            BtnLogin.Enabled = true;
            label1.Visible = true;
            label2.Visible = true;
            MostrarPass.Visible = true;
            BtnRegistrar.Visible = true;
            BtnLogin.Visible = true;
            btnConectar.Enabled = false;
            btnDesconectar.Visible = true;
            label5.Visible = true;
            label6.Visible = true;

            //Creamos un IPEndPoint con el ip del servidor y puerto del servidor 
            //al que deseamos conectarnos
            IPAddress direc = IPAddress.Parse("192.168.56.102");
            IPEndPoint ipep = new IPEndPoint(direc, 9070);

            // IPAddress direc = IPAddress.Parse("147.83.117.22");
            // IPEndPoint ipep = new IPEndPoint(direc, 50000);


            //Creamos el socket 
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Connect(ipep);//Intentamos conectar el socket
                MessageBox.Show("Conectado correctamente");

            }
            catch (SocketException ex)
            {
                //Si hay excepcion imprimimos error y salimos del programa con return 
                MessageBox.Show("No he podido conectar con el servidor");
                return;
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            //Mensaje de desconexión
            string mensaje = "0/";

            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            // Nos desconectamos
            server.Shutdown(SocketShutdown.Both);
            server.Close();

        }

        private void EnviarBtn_Click(object sender, EventArgs e)
        {
            if (radioButton1.Checked)
            {
                string mensaje = "9/" + dia.Text;
                // Enviamos al servidor el dia tecleado
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                if (mensaje == "mal")
                {
                    MessageBox.Show("Ese dia no se jugó ninguna partida");
                }
                else
                {
                    MessageBox.Show("El nombre del ganador y la duración de la partida es: " + mensaje);

                }
            }
            else if (radioButton2.Checked)
            {
                string mensaje = "10/" + nombre.Text;
                // Enviamos al servidor el nombre tecleado
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                if (mensaje == "mal")
                {
                    MessageBox.Show("Este jugador no ha ganado ninguna partida");
                }
                else
                {
                    MessageBox.Show("Las partidas ganadas de este jugador son: " + mensaje);
                }
            }
            else if (radioButton3.Checked)
            {
                // Enviamos el nombre
                string mensaje = "11/" + nombre.Text;
                // Enviamos al servidor el nombre tecleado
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                if (mensaje == "mal")
                {
                    MessageBox.Show("Este jugador no ha jugado ninguna partida");
                }
                else
                {
                    MessageBox.Show("Este jugador ha jugado el siguiente número de partidas: " + mensaje);
                }
            }
            else if (radioButton4.Checked)
            {
                string mensaje = "12/" + nombre.Text;
                // Enviamos al servidor el nombre tecleado
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                if (mensaje == "mal")
                {
                    MessageBox.Show("No se ha encontrado ninguna ID con ese nombre");
                }
                else
                MessageBox.Show("La ID del jugador es:" + mensaje);

            }
        }

        private void BtnRegistrar_Click(object sender, EventArgs e)
        {
            if (((username.Text.Length > 1 && password.Text.Length > 1)) && ((username.Text != "") && (password.Text != "")))
            {
                string mensaje = "8/" + username.Text + "/" + password.Text;
                // // Enviamos al servidor el nombre tecleado
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                if (mensaje == "SI")
                {
                    BtnRegistrar.Enabled = false;              
                    MessageBox.Show("Jugador registrado correctamente, ya puedes iniciar sesión");
                }                   
                else
                    MessageBox.Show("Este jugador ya está registrado");
            }
            else
            {
                MessageBox.Show("El nombre y la contraseña deben tener más de un carácter");

            }
        }

        private void BtnLogin_Click(object sender, EventArgs e)
        {
            
                if ((username.Text != "") && (password.Text != ""))
                {
                string mensaje = "7/" + username.Text + "/" + password.Text;
                    // Enviamos al servidor el nombre tecleado
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);

                    //Recibimos la respuesta del servidor
                    byte[] msg2 = new byte[80];
                    server.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];

                    if (mensaje != "NO")
                    {
                    BtnLogin.Enabled = false;
                    peticion.Visible = true;                    
                    btn_lista_conectados.Visible = true;
                    BtnRegistrar.Enabled = false;
                    numConsultas.Visible = true;
                    contConsultas.Visible = true;
                    username.Enabled = false;
                    password.Enabled = false;
                    ID_J = Convert.ToInt32(mensaje);
                        MessageBox.Show("Sesión iniciada correctamente");
                    }
                    else
                    {
                        MessageBox.Show("Usuario mal escrito, escribe bien el usuario y la contraseña, o " +
                             "registrate para poder jugar a este maravilloso juego");
                    }

                }
                else
                {
                    MessageBox.Show("El campo de usuario o contraseña está vacio");
                }
        }

        private void btn_lista_conectados_Click(object sender, EventArgs e)
        {

                dataGridView1.Visible = true;
                dataGridView1.Rows.Clear();

                //Queremos saber la lista de conectados
                string mensaje = "13/";
                //Enviamos en el servidor el nombre como un bytes
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                //Convertir los bytes en ASCII (proceso inverso)
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];

                if (mensaje != null && mensaje != "")
                {
                    string[] porcadausername = mensaje.Split(',');
                    int num = Convert.ToInt32(porcadausername[0]);
                    int i = 0;
                    while (i < num)
                    {
                        int x = dataGridView1.Rows.Add();
                        dataGridView1.Rows[x].Cells[0].Value = porcadausername[i+1];
                        i++;
                    }
                }
                else
                {
                    MessageBox.Show("Ha habido algo raro");
                }
           
        }

        private void MostrarPass_CheckedChanged(object sender, EventArgs e)
        {
            if (MostrarPass.Checked == false)
            {
                password.UseSystemPasswordChar = false;

            }
            else
            {
                password.UseSystemPasswordChar = true;
            }
        }

        private void numConsultas_Click(object sender, EventArgs e)
        {
            // Pedir numero de servicios realizados
            string mensaje = "20/";
            // Enviamos al servidor el nombre tecleado
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            //Recibimos la respuesta del servidor
            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
            contConsultas.Text = mensaje;
        }
    }
}
