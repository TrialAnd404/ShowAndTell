package de.hsrm.cs.wwwvs.hamster.rpc.cli_client;

public class HamsterEntry {
	public String id;
	public String owner;
	public String name;
	public String price;
	public String treats;
	
	public HamsterEntry(String id, String owner, String name, String price, String treats) {
		this.id = id;
		this.owner = owner;
		this.name = name;
		this.price = price;
		this.treats = treats;
	}
}
