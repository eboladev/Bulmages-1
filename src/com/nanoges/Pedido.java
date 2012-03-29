package com.nanoges;

public class Pedido {

		private long id;
		private String tablename = "pedidocliente";
		private String idcliente;
		private String desc;
		private String fecha;
		private String coment;
		
		public long getId() {
			return id;
		}

		public String getIdCliente() {
			return idcliente;
		}
		public String getDesc() {
			return desc;
		}
		
		public String getFecha() {
			return fecha;
		}
		
		public String getComent() {
			return coment;
		}
		
		
		public void setId(long id) {
			this.id = id;
		}

		public String getPedido() {
			return desc;
		}

		public void setPedido(String midcliente, String mdesc, String mfecha, String mcoment) {
			this.idcliente = midcliente;
			this.desc = mdesc;
			this.fecha = mfecha;
			this.coment= mcoment;
		}

		// Will be used by the ArrayAdapter in the ListView
		@Override
		public String toString() {
			return fecha + ".-" + desc;
		}
	}