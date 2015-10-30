require 'packetfu'

if (ARGV.length == 0)
    cap = PacketFu::Capture.new(:start => true, :iface => 'eth0', :promisc => true)
    #stream.show_live()
    numIncident = 0
    cap.stream.each do |p|
      pkt = PacketFu::TCPPacket.parse p
      if pkt.is_ip?
        if defined? pkt.tcp_flags
        #packet_info = [pkt.ip_saddr, pkt.ip_daddr, pkt.size, pkt.proto.last]
          didSomethingHappen = false
          if (pkt.tcp_flags.urg == 1 && pkt.tcp_flags.fin == 1 && pkt.tcp_flags.psh == 1)
            typeOfScan = "Xmas"
            didSomethingHappen = true
          end
          if (pkt.tcp_flags.urg == 0 && pkt.tcp_flags.fin == 0 && pkt.tcp_flags.psh == 0 && pkt.tcp_flags.ack == 0 && pkt.tcp_flags.rst == 0 && pkt.tcp_flags.syn == 0)
            typeOfScan = "NULL"
            didSomethingHappen = true
          end
          if (/4\d{3}(\s|-)?\d{4}(\s|-)?\d{4}(\s|-)?\d{4}/.match(pkt.payload)) || (/5\d{3}(\s|-)?\d{4}(\s|-)?\d{4}(\s|-)?\d{4}/.match(pkt.payload)) || (/6011(\s|-)?\d{4}(\s|-)?\d{4}(\s|-)?\d{4}/.match(pkt.payload)) || (/3\d{3}(\s|-)?\d{6}(\s|-)?\d{5}/.match(pkt.payload))
            puts "#{numIncident}. ALERT: Credit card leaked in the clear from #{pkt.ip_saddr} (#{pkt.proto.last}) (#{pkt.payload})!"
            numIncident = numIncident + 1
          end
          if (didSomethingHappen == true)
            puts "#{numIncident}. ALERT: #{typeOfScan} scan is detected from #{pkt.ip_saddr} (#{pkt.proto.last}) (#{pkt.payload})!"
            numIncident = numIncident + 1
          end
        end
      end
  end
end
if (ARGV.length > 0)
  if (ARGV[0] == '-r')
    numIncident = 0
    toAnalyze = []
    File.open(ARGV[1]) do |f|
      f.lines.each do |line|
        toAnalyze << line
      end
    end
    
    toAnalyze.each do |p|
      didSomethingHappen = false
      statusCodeSpot = p[(p.index('" ') + 2)..-1]
      if statusCodeSpot[0] == '4'
        attack = "HTTP Error"
        didSomethingHappen = true
      end
      payload = p[(p.index(' "') + 1)..p.index('" ')]
      if /\\x\w{2}(\w)?\\x/.match(payload)
        attack = "Shellcode"
        didSomethingHappen = true
      end
      if (p.include? 'nmap') || (p.include? 'Nmap')
        attack = "NMAP scan"
        didSomethingHappen = true
      end
      #TODO: shellcode stuff here
      if didSomethingHappen == true
        mIpAddressEnd = p.index(' - ')
        mIpAddress = p[0..mIpAddressEnd]
        payload = p[(p.index(' "') + 1)..p.index('" ')]
        #puts payload
        #puts attack
        if payload.index(' ') != nil && payload.index(' ', (payload.index(' ') + 1)) != nil
          secondSpaceOccurence = payload.index(' ', (payload.index(' ') + 1))
          mIndex = secondSpaceOccurence + 1
          protocol = payload[(mIndex)..-1]
          if protocol.index('/') != nil
            protocol = protocol[0..(protocol.index('/') - 1)]
          else
            protocol = protocol[0..3]
          end
        else
          protocol = 'HTTP'
        end
        
        puts "#{numIncident}. ALERT: #{attack} is detected from #{mIpAddress} (#{protocol}) (#{payload})"
        numIncident = numIncident + 1
      end
    end
  end
end
