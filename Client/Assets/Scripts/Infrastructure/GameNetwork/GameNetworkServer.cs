using System.Collections.Generic;
using System.Linq;
using System.Threading;
using UnityEngine;
using Google.FlatBuffers;
using System;
using PacketDefine;

namespace Assets.Scripts.GameNetwork
{
    public class GameNetworkServer : MonoBehaviour
    {
        private static GameNetworkServer instance = null;
        private string address = "127.0.0.1";

        private int port = Convert.ToInt32("11000");
        const int PacketHeaderSize = 5;

        ClientSimpleTcp Network = new ClientSimpleTcp();



        PacketBufferManager PacketBuffer = new PacketBufferManager();

        Queue<byte[]> RecvPacketQueue = new Queue<byte[]>();
        Queue<byte[]> SendPacketQueue = new Queue<byte[]>();
        //  public Queue<RoomChatNotPacket> ChatMsgQueue { get; set; } = new Queue<RoomChatNotPacket>();

        bool IsNetworkThreadRunning = false;

        public bool GetIsConnected() { return Network.IsConnected(); }
        public void Disconnect() { Network.Close(); }

        System.Threading.Thread NetworkReadThread = null;
        System.Threading.Thread NetworkSendThread = null;
        System.Threading.Thread ProcessReceivedPacketThread = null;


        public CLIENT_STATUS ClientStatus { get; set; } = new CLIENT_STATUS();
        public string UserID { get; set; } = "";
        public string RivalID { get; set; } = "";

        public Single SyncPacketInterval { get; set; } = 0.1f;


        public enum CLIENT_STATUS
        {
            NONE = 0,
            LOGIN = 1,
            ROOM = 2,
            GAME = 3,
        }


        public static GameNetworkServer Instance
        {
            get
            {
                return instance;
            }
        }

        void Awake()
        {
            if (instance)
            {
                DestroyImmediate(gameObject);
                return;
            }
            instance = this;
            Init();
            DontDestroyOnLoad(gameObject);
        }


        void Init()
        {
            ClientStatus = CLIENT_STATUS.NONE;

            PacketBuffer.Init((8096 * 10), PacketHeaderSize, 1024);

            IsNetworkThreadRunning = true;
            NetworkReadThread = new System.Threading.Thread(this.NetworkReadProcess);
            NetworkReadThread.Start();
            NetworkSendThread = new System.Threading.Thread(this.NetworkSendProcess);
            NetworkSendThread.Start();
            ProcessReceivedPacketThread = new System.Threading.Thread(this.ProcessReceivedPacket);
            ProcessReceivedPacketThread.Start();
        }

        public void StopAllNetWorkThread()
        {
            NetworkReadThread.Join();
            NetworkSendThread.Join();
            ProcessReceivedPacketThread.Join();
        }

        public void StartAllNetWorkThread()
        {
            NetworkReadThread.Start();
            NetworkSendThread.Start();
            ProcessReceivedPacketThread.Start();
        }

        //게임서버 네트워크 부분
        public void ConnectToServer(string ip_address, int port_val)
        {
            if (Network.Connect(ip_address, port_val))
            {
                Debug.Log("접속성공!");
            }
            else
            {
                Debug.Log("접속실패");
            }
        }

        public void RequestLogin(string loginID, string loginPW)
        {
            FlatBufferBuilder fb = new FlatBufferBuilder(1024); // size 조절
            var id = fb.CreateString(loginID);
            var pw = fb.CreateString(loginPW);

            PACKET_LOGIN_REQ.StartPACKET_LOGIN_REQ(fb);
            PACKET_LOGIN_REQ.AddUsedId(fb, id);
            PACKET_LOGIN_REQ.AddPassword(fb, pw);
            var packetOffset = PACKET_LOGIN_REQ.EndPACKET_LOGIN_REQ(fb);

            Packet.StartPacket(fb);
            Packet.AddPacketIdType(fb, PACKET_ID.PACKET_LOGIN_REQ);
            Packet.AddPacketId(fb, packetOffset.Value);
            var rootOffset = Packet.EndPacket(fb);

            Packet.FinishPacketBuffer(fb, rootOffset);

            PostSendPacket(PACKET_ID.PACKET_LOGIN_REQ, fb.SizedByteArray());
        }


        // 게임플레이 네트워크 부분
        /*        public void SendGameStartPacket(GameStartRequestPacket packet)
                {
                    var request = packet;
                    PostSendPacket(PACKET_ID.GameStartReqPkt, null);
                }*/


        //네트워크 Read/Send 스레드 부분
        void PostSendPacket(PACKET_ID packetID, byte[] bodyData)
        {
            // 역직렬화
            var buf = new ByteBuffer(bodyData);
            var packet = Packet.GetRootAsPacket(buf);

            var packet_id = packet.PacketIdType;

            if (packet_id == PACKET_ID.PACKET_LOGIN_REQ)
            {
                var login_packet = packet.PacketId<PACKET_LOGIN_REQ>().Value;
                var user_id = login_packet.UsedId;
                var user_pw = login_packet.Password;
            }

            if (Network.IsConnected() == false)
            {
                Debug.LogWarning("서버에 접속하지 않았습니다");
                return;
            }

            List<byte> dataSource = new List<byte>();
            var packetSize = 0;

            if (bodyData != null)
            {
                packetSize = (Int16)(bodyData.Length + PacketHeaderSize);
            }
            else
            {
                packetSize = (Int16)(PacketHeaderSize);
            }

            dataSource.AddRange(BitConverter.GetBytes((Int16)packetSize));
            dataSource.AddRange(BitConverter.GetBytes((Int16)packetID));
            dataSource.AddRange(new byte[] { (byte)0 });
            if (bodyData != null)
            {
                //dataSource.AddRange(bodyData);
            }
            SendPacketQueue.Enqueue(dataSource.ToArray());
        }



        void NetworkReadProcess()
        {
            while (IsNetworkThreadRunning)
            {
                System.Threading.Thread.Sleep(32);

                if (Network.IsConnected() == false)
                {
                    continue;
                }

                var recvData = Network.Receive();

                if (recvData.Count > 0)
                {
                    PacketBuffer.Write(recvData.Array, recvData.Offset, recvData.Count);

                    while (true)
                    {
                        var data = PacketBuffer.Read();
                        if (data.Count < 1)
                        {
                            break;
                        }
                        var buf = new ByteBuffer(data.Array);
                        FlatBufferBuilder fb = new FlatBufferBuilder(buf); // size 조절

                        // https://stackoverflow.com/questions/35397500/any-way-to-determine-what-kind-of-schema-was-received
                        // Offset을 떼서 ID가져올 수 있다

                        /*
                        var packet = new PacketData();

                        packet.DataSize = (short)(data.Count - PacketHeaderSize);
                        packet.PacketID = BitConverter.ToInt16(data.Array, data.Offset + 2);
                        packet.BodyData = new byte[packet.DataSize];
                        Buffer.BlockCopy(data.Array, (data.Offset + PacketHeaderSize), packet.BodyData, 0, (data.Count - PacketHeaderSize));

                        lock (((System.Collections.ICollection)RecvPacketQueue).SyncRoot)
                        {
                            RecvPacketQueue.Enqueue(packet);
                        }
                        */
                    }
                }
                else
                {
                    /*
                    var packet = new PacketData();
                    packet.PacketID = (short)PACKET_ID.SYSTEM_CLIENT_DISCONNECTD;
                    packet.DataSize = 0;

                    lock (((System.Collections.ICollection)RecvPacketQueue).SyncRoot)
                    {
                        RecvPacketQueue.Enqueue(packet);
                    }
                    */
                }
            }
        }

        void NetworkSendProcess()
        {
            while (IsNetworkThreadRunning)
            {
                System.Threading.Thread.Sleep(32);

                if (Network.IsConnected() == false)
                {
                    continue;
                }

                lock (((System.Collections.ICollection)RecvPacketQueue).SyncRoot)
                {
                    if (SendPacketQueue.Count > 0)
                    {
                        var packet = SendPacketQueue.Dequeue();
                        //    Debug.Log("SendPacket Packet ID=" + packet.ToString());
                        Network.Send(packet);
                    }
                }
            }
        }


        void ProcessReceivedPacket()
        {
            while (IsNetworkThreadRunning)
            {
                System.Threading.Thread.Sleep(32);
                ReadPacketQueueProcess();
            }
        }


        void ReadPacketQueueProcess()
        {
            try
            {
                /*
                PacketData packet = new PacketData();
                lock (((System.Collections.ICollection)RecvPacketQueue).SyncRoot)
                {
                    if (RecvPacketQueue.Count() > 0)
                    {
                        packet = RecvPacketQueue.Dequeue();
                    }
                }

                if (packet.PacketID != 0)
                {
                    GameServerPacketHandler.Process(packet);
                }
                */
            }
            catch (Exception ex)
            {
                Debug.LogError(ex.Message);
            }
        }

    }
}
